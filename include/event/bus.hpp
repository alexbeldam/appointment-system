#ifndef BUS_HPP
#define BUS_HPP

#include <functional>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>

/**
 * @brief Um barramento de eventos thread-safe para comunicação desacoplada.
 *
 * Gerencia a inscrição e a publicação de eventos de diversos tipos.
 */
class EventBus {
   private:
    /**
     * @brief Mapa de manipuladores (handlers) inscritos por tipo de evento.
     *
     * A chave é o `std::type_index` do evento. O valor é um vetor de funções
     * que recebem um ponteiro constante para o evento.
     */
    std::unordered_map<std::type_index,
                       std::vector<std::function<void(const void*)>>>
        subscribers;

    /**
     * @brief Mutex para garantir a thread-safety das operações.
     */
    std::mutex mx;

   public:
    /**
     * @brief Alias para o tipo de manipulador de evento.
     * @tparam EventType O tipo da estrutura ou classe do evento.
     */
    template <typename EventType>
    using Handler = std::function<void(const EventType&)>;

    /**
     * @brief Inscreve uma função manipuladora (handler) para um tipo de evento.
     * @tparam EventType O tipo do evento.
     * @param handler A função a ser executada ao receber o evento.
     */
    template <typename EventType>
    void subscribe(Handler<EventType> handler) {
        std::lock_guard<std::mutex> lock(mx);

        auto& handlers = subscribers[std::type_index(typeid(EventType))];

        handlers.push_back([handler](const void* e) {
            handler(*static_cast<const EventType*>(e));
        });
    }

    /**
     * @brief Publica um evento no barramento, notificando todos os inscritos.
     * @tparam EventType O tipo do evento.
     * @param event A instância do evento a ser publicada.
     */
    template <typename EventType>
    void publish(const EventType& event) {
        std::lock_guard<std::mutex> lock(mx);

        auto it = subscribers.find(std::type_index(typeid(EventType)));

        if (it == subscribers.end())
            return;

        for (auto& fn : it->second)
            fn(&event);
    }
};

#endif
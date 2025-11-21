#ifndef BUS_HPP
#define BUS_HPP

#include <functional>
#include <mutex>
#include <typeindex>
#include <unordered_map>

/**
 * @brief Implementa um mecanismo de Barramento de Eventos (Event Bus) síncrono.
 * * Segue o padrão Publicador/Assinante (Publisher/Subscriber), permitindo
 * que componentes do sistema se comuniquem de forma desacoplada através
 * da publicação e consumo de eventos.
 */
class EventBus {
   private:
    /**
     * @brief Mapa que armazena os manipuladores (handlers) de eventos.
     * * A chave é o tipo do evento (std::type_index).
     * * O valor é um vetor de funções que manipulam o evento, armazenadas
     * como std::function<void(const void*)> para uniformidade.
     */
    std::unordered_map<std::type_index,
                       std::vector<std::function<void(const void*)>>>
        subscribers;

    /**
     * @brief Mutex para garantir o acesso thread-safe às operações de
     * inscrição e publicação.
     */
    std::mutex mx;

   public:
    /**
     * @brief Alias de tipo para um manipulador de eventos.
     * * Representa uma função que aceita uma referência constante a um tipo
     * específico de evento.
     * * @tparam EventType O tipo da classe de evento.
     */
    template <typename EventType>
    using Handler = std::function<void(const EventType&)>;

    /**
     * @brief Registra um manipulador para um tipo de evento específico.
     * * O manipulador (handler) será chamado sempre que um evento do tipo
     * EventType for publicado.
     * * @tparam EventType O tipo da classe de evento a ser inscrita.
     * @param handler A função a ser chamada quando o evento ocorrer.
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
     * @brief Publica um evento, disparando todos os manipuladores inscritos.
     * * Se houver manipuladores registrados para o EventType, eles serão
     * executados síncronamente.
     * * @tparam EventType O tipo da classe de evento que está sendo publicada.
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
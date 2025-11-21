#ifndef ENTITY_LIST_HPP
#define ENTITY_LIST_HPP

#include <functional>
#include <memory>
#include <vector>

/**
 * @brief Alias de tipo para funções de carregamento de lista de entidades.
 * * Usada para carregar coleções de entidades associadas a um ID de
 * proprietário.
 * @tparam T O tipo da entidade (ex: Agendamento, Horario).
 * @return std::vector<std::shared_ptr<T>> Um vetor de ponteiros inteligentes
 * para as entidades.
 * @param long O ID do proprietário (ex: Aluno ID, Professor ID).
 */
template <typename T>
using ListLoaderFunction = std::function<std::vector<std::shared_ptr<T>>(long)>;

/**
 * @brief Alias de tipo para funções de carregamento de entidade única.
 * * Usada para carregar uma única entidade pelo seu ID.
 * @tparam T O tipo da entidade (ex: Aluno, Professor).
 * @return std::shared_ptr<T> Um ponteiro inteligente para a entidade.
 * @param long O ID da entidade.
 */
template <typename T>
using LoadFunction = std::function<std::shared_ptr<T>(long)>;

/**
 * @brief Implementa o padrão Lazy Loading para listas de entidades.
 * * Esta classe armazena uma função de carregamento (ListLoaderFunction) e só
 * executa a consulta de dados quando a lista é acessada pela primeira vez (ex:
 * size(), begin(), operator[]).
 * @tparam T O tipo da entidade armazenada.
 */
template <typename T>
class EntityList {
   private:
    /**
     * @brief A função de callback responsável por carregar os dados.
     */
    ListLoaderFunction<T> loaderFunction;

    /**
     * @brief O contêiner para armazenar os dados carregados.
     */
    std::vector<std::shared_ptr<T>> data;

    /**
     * @brief Flag que indica se os dados já foram carregados.
     */
    bool isLoaded = false;

    /**
     * @brief O ID da entidade proprietária (ex: Aluno ID) usado como parâmetro
     * para o loader.
     */
    long ownerId;

    /**
     * @brief Executa o carregamento dos dados se ainda não tiverem sido
     * carregados.
     */
    void loadData() {
        if (!isLoaded && loaderFunction) {
            data = loaderFunction(ownerId);
            isLoaded = true;
        }
    }

   public:
    /**
     * @brief Alias para o tipo do vetor interno de entidades.
     */
    using EntityVector = std::vector<std::shared_ptr<T>>;

    /**
     * @brief Alias para o iterador mutável do vetor de entidades.
     */
    using Iterator = typename EntityVector::iterator;

    /**
     * @brief Construtor da classe EntityList.
     * @param loader A função de carregamento que será executada sob demanda.
     * @param ownerId O ID do proprietário, passado para a função de
     * carregamento.
     */
    EntityList(ListLoaderFunction<T> loader, long ownerId)
        : loaderFunction(loader), ownerId(ownerId) {}

    /**
     * @brief Destrutor padrão.
     */
    ~EntityList() = default;

    /**
     * @brief Operador de acesso.
     * * Dispara o carregamento preguiçoso (loadData) antes de retornar o
     * elemento.
     * @param index O índice do elemento.
     * @return std::shared_ptr<T> O elemento no índice especificado.
     */
    std::shared_ptr<T> operator[](size_t index) {
        loadData();

        return data[index];
    }

    /**
     * @brief Retorna um iterador para o início da lista.
     * * Dispara o carregamento preguiçoso.
     * @return Iterator O iterador inicial.
     */
    Iterator begin() {
        loadData();

        return data.begin();
    }

    /**
     * @brief Retorna um iterador para o final da lista.
     * * Dispara o carregamento preguiçoso.
     * @return Iterator O iterador final.
     */
    Iterator end() {
        loadData();

        return data.end();
    }

    /**
     * @brief Retorna o número de elementos na lista.
     * * Dispara o carregamento preguiçoso.
     * @return size_t O tamanho da lista.
     */
    size_t size() {
        loadData();

        return data.size();
    }

    /**
     * @brief Verifica se a lista está vazia.
     * * Dispara o carregamento preguiçoso.
     * @return bool True se a lista estiver vazia.
     */
    bool empty() {
        return size() == 0;
    }
};

#endif
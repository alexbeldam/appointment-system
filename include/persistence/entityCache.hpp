#ifndef ENTITY_CACHE_HPP
#define ENTITY_CACHE_HPP

#include <map>
#include <memory>

#include "util/fileObserver.hpp"

/**
 * @brief Implementa um cache genérico para entidades do sistema.
 * * Este cache armazena objetos de entidades (T) indexados por um ID numérico
 * (long).
 * * O cache suporta **invalidação automática** baseada na observação de
 * arquivos de dados, garantindo que os dados em memória sejam atualizados
 * quando os arquivos persistidos mudarem.
 * @tparam T O tipo da entidade a ser armazenada (ex: Aluno, Professor).
 */
template <typename T>
class EntityCache {
   private:
    /**
     * @brief O contêiner de armazenamento principal do cache.
     * * Utiliza std::map para armazenar ponteiros inteligentes de T, indexados
     * pelo ID (long).
     */
    std::map<long, std::shared_ptr<T>> cache;

    /**
     * @brief Observador de arquivos que monitora a mudança nos arquivos de
     * dados.
     * * Usado para decidir se o cache deve ser invalidado.
     */
    FileObserver fileObserver;

   public:
    /**
     * @brief Alias para o tipo do mapa interno de cache.
     */
    using CacheMap = std::map<long, std::shared_ptr<T>>;

    /**
     * @brief Alias para o iterador mutável do cache.
     */
    using Iterator = typename CacheMap::iterator;

    /**
     * @brief Alias para o iterador constante do cache.
     */
    using ConstIterator = typename CacheMap::const_iterator;

    /**
     * @brief Construtor da classe EntityCache.
     * * Inicializa o observador de arquivos com a lista de arquivos a serem
     * monitorados.
     * @param observedFiles Um vetor de strings contendo os caminhos dos
     * arquivos de dados que, se alterados, devem invalidar este cache.
     */
    EntityCache(const std::vector<std::string>& observedFiles)
        : fileObserver(observedFiles) {}

    /**
     * @brief Destrutor padrão.
     */
    ~EntityCache() = default;

    /**
     * @brief Verifica se algum dos arquivos observados foi alterado e, se sim,
     * invalida (limpa) o cache.
     * @return bool Retorna true se o cache foi limpo devido a uma mudança no
     * arquivo, false caso contrário.
     */
    bool invalidate() {
        if (fileObserver.hasFileChanged()) {
            cache.clear();
            return true;
        }

        return false;
    }

    /**
     * @brief Verifica se uma entidade com o ID especificado está presente no
     * cache.
     * @param id O identificador único da entidade.
     * @return bool True se o ID estiver no cache, false caso contrário.
     */
    bool contains(long id) const {
        return cache.count(id) > 0;
    }

    /**
     * @brief Retorna um ponteiro inteligente para a entidade com o ID
     * especificado.
     * @param id O identificador único da entidade.
     * @return std::shared_ptr<T> O ponteiro para a entidade.
     * @throws std::out_of_range Se o ID não for encontrado no cache.
     */
    std::shared_ptr<T> at(long id) {
        if (contains(id))
            return cache[id];

        throw std::out_of_range("Id " + std::to_string(id) +
                                " não encontrado no cache");
    }

    /**
     * @brief Retorna um iterador constante para o início do cache.
     * @return ConstIterator O iterador.
     */
    ConstIterator begin() const {
        return cache.begin();
    }

    /**
     * @brief Retorna um iterador constante para o final do cache.
     * @return ConstIterator O iterador.
     */
    ConstIterator end() const {
        return cache.end();
    }

    /**
     * @brief Retorna um iterador mutável para o início do cache.
     * @return Iterator O iterador.
     */
    Iterator begin() {
        return cache.begin();
    }

    /**
     * @brief Retorna um iterador mutável para o final do cache.
     * @return Iterator O iterador.
     */
    Iterator end() {
        return cache.end();
    }

    /**
     * @brief Retorna o número de entidades armazenadas no cache.
     * @return size_t O tamanho atual do cache.
     */
    size_t size() const {
        return cache.size();
    }

    /**
     * @brief Insere ou atualiza uma entidade no cache.
     * @param id O identificador único da entidade.
     * @param entity O ponteiro inteligente para a entidade.
     */
    void put(long id, std::shared_ptr<T> entity) {
        cache[id] = entity;
    }

    /**
     * @brief Remove uma entidade do cache pelo seu ID.
     * @param id O identificador único da entidade a ser removida.
     */
    void erase(long id) {
        cache.erase(id);
    }
};

#endif
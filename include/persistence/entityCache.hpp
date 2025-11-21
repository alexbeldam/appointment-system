#ifndef ENTITY_CACHE_HPP
#define ENTITY_CACHE_HPP

#include <map>
#include <memory>

#include "util/fileObserver.hpp"

template <typename T>
class EntityCache {
   private:
    std::map<long, std::shared_ptr<T>> cache;
    FileObserver fileObserver;

   public:
    using CacheMap = std::map<long, std::shared_ptr<T>>;
    using Iterator = typename CacheMap::iterator;
    using ConstIterator = typename CacheMap::const_iterator;

    EntityCache(const std::vector<std::string>& observedFiles)
        : fileObserver(observedFiles) {}

    ~EntityCache() = default;

    bool invalidate() {
        if (fileObserver.hasFileChanged()) {
            cache.clear();
            return true;
        }

        return false;
    }

    bool contains(long id) const {
        return cache.count(id) > 0;
    }

    std::shared_ptr<T> at(long id) {
        if (contains(id))
            return cache[id];

        throw std::out_of_range("Id " + std::to_string(id) +
                                " não encontrado no cache");
    }

    ConstIterator begin() const {
        return cache.begin();
    }

    ConstIterator end() const {
        return cache.end();
    }

    Iterator begin() {
        return cache.begin();
    }

    Iterator end() {
        return cache.end();
    }

    size_t size() const {
        return cache.size();
    }

    void put(long id, std::shared_ptr<T> entity) {
        cache[id] = entity;
    }

    void erase(long id) {
        cache.erase(id);
    }
};

#endif
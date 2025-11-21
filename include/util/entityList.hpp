#ifndef ENTITY_LIST_HPP
#define ENTITY_LIST_HPP

#include <functional>
#include <memory>
#include <vector>

template <typename T>
using ListLoaderFunction = std::function<std::vector<std::shared_ptr<T>>(long)>;

template <typename T>
using LoadFunction = std::function<std::shared_ptr<T>(long)>;

template <typename T>
class EntityList {
   private:
    ListLoaderFunction<T> loaderFunction;
    std::vector<std::shared_ptr<T>> data;
    bool isLoaded = false;
    long ownerId;

    void loadData() {
        if (!isLoaded && loaderFunction) {
            data = loaderFunction(ownerId);
            isLoaded = true;
        }
    }

   public:
    using EntityVector = std::vector<std::shared_ptr<T>>;
    using Iterator = typename EntityVector::iterator;

    EntityList(ListLoaderFunction<T> loader, long ownerId)
        : loaderFunction(loader), ownerId(ownerId) {}

    ~EntityList() = default;

    std::shared_ptr<T> operator[](size_t index) {
        loadData();

        return data[index];
    }

    Iterator begin() {
        loadData();

        return data.begin();
    }

    Iterator end() {
        loadData();

        return data.end();
    }

    size_t size() {
        loadData();

        return data.size();
    }

    bool empty() {
        return size() == 0;
    }
};

#endif
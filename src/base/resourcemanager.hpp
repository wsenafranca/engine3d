//
// Created by Washington on 22/02/2020.
//

#ifndef ENGINE3D_SRC_RESOURCEMANAGER_HPP
#define ENGINE3D_SRC_RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <algorithm>
#include <mutex>

template<class T, int CacheSize>
class ResourceManager {
public:
    ~ResourceManager() {
        for(auto it : mObjects) {
            delete it.second.first;
        }
    }

    template<class... Args>
    T* Create(const std::string& name, Args&& ...args) {
        std::lock_guard<std::mutex> lock(mMutex);
        auto it = mObjects.find(name);
        if(it != mObjects.end()) {
            it->second.second++;
            return it->second.first;
        }
        auto object = new T(args...);
        mObjects[name] = std::pair<T*, uint32_t>(object, 1);
        return object;
    }

    [[nodiscard]] bool HasObject(const std::string& name) const {
        return mObjects.find(name) != mObjects.end();
    }

    [[nodiscard]] T* Get(const std::string& name) const {
        auto it = mObjects[name];
        it.second++;
        return it.first;
    }

    void Destroy(T* object) {
        if(!object) return;
        std::lock_guard<std::mutex> lock(mMutex);
        for(auto it = mObjects.begin(); it != mObjects.end(); ++it) {
            if(it->second.first == object) {
                if((--(it->second.second)) == 0) {
                    if(mCache.size() < CacheSize) {
                        mCache.push(std::pair<std::string, T*>(it->first, it->second.first));
                    } else {
                        auto &oldest = mCache.front();
                        auto cacheIt = mObjects.find(oldest.first);
                        if(cacheIt != mObjects.end()) {
                            mObjects.erase(cacheIt);
                        }
                        delete it->second.first;
                        mCache.pop();
                    }

                    return;
                }
            }
        }
    }

private:
    mutable std::unordered_map< std::string, std::pair<T*, uint32_t> > mObjects;
    mutable std::queue< std::pair<std::string, T*> > mCache;
    std::mutex mMutex;
};


#endif //ENGINE3D_SRC_RESOURCEMANAGER_HPP

//
// Created by Washington on 23/02/2020.
//

#ifndef ENGINE3D_SRC_POOL_HPP
#define ENGINE3D_SRC_POOL_HPP

#include <list>
#include <cstdint>
#include <mutex>

template<class T, uint32_t PoolSize>
class Pool {
public:
    ~Pool() {
        for(auto& object : mAcquired) {
            delete object;
        }

        for(auto& object : mReleased) {
            delete object;
        }
    }

    template<class... Args>
    T* Create(Args&& ...args) {
        T* object;
        {
            std::lock_guard<std::mutex> lock(mReleasedMutex);
            if(!mReleased.empty()) {
                object = mReleased.front();
                mReleased.pop_front();
            } else {
                object = new T();
            }
        }
        object->OnCreate(args...);
        {
            std::lock_guard<std::mutex> lock(mAcquiredMutex);
            mAcquired.push_back(object);
        }
        return object;
    }

    void Destroy(T* object) {
        object->OnDestroy();
        {
            std::lock_guard<std::mutex> lock(mAcquiredMutex);
            mAcquired.remove(object);
        }
        {
            std::lock_guard<std::mutex> lock(mReleasedMutex);
            if(mReleased.size() < PoolSize) {
                mReleased.push_back(object);
            }
        }
    }

private:
    std::list<T*> mAcquired;
    std::list<T*> mReleased;
    std::mutex mAcquiredMutex;
    std::mutex mReleasedMutex;
};

#endif //ENGINE3D_SRC_POOL_HPP

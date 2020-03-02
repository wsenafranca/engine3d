//
// Created by Washington on 26/02/2020.
//

#ifndef ENGINE3D_SRC_SINGLETON_HPP
#define ENGINE3D_SRC_SINGLETON_HPP

template<class T>
class Singleton {
public:
    static T* GetInstance() {
        static T sInstance;
        return &sInstance;
    }

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif //ENGINE3D_SRC_SINGLETON_HPP

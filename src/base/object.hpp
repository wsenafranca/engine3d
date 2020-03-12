//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_OBJECT_HPP
#define ENGINE3D_SRC_OBJECT_HPP

#include <string>
#include <unordered_map>
#include <memory>

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] const std::string &GetName() const;
    void SetName(const std::string &name);

    template<class T>
    T* As() {
        return dynamic_cast<T*>(this);
    }

    template<class T>
    [[nodiscard]] const T* As() const {
        return dynamic_cast<const T*>(this);
    }

protected:
    std::string mName;
};

#endif //ENGINE3D_SRC_OBJECT_HPP

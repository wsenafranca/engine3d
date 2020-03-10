//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_OBJECT_HPP
#define ENGINE3D_SRC_OBJECT_HPP

#include <string>
#include <unordered_map>
#include <any>
#include <optional>
#include <memory>

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] const std::string &GetName() const;
    void SetName(const std::string &name);

    const std::unordered_map<std::string, std::any> &GetProperties() const;

    [[nodiscard]] bool HasProperty(const std::string& name) const;

    template<class T>
    void SetProperty(const std::string& name, const T& value) {
        mProperties[name] = value;
    }

    template<class T>
    [[nodiscard]] std::optional<T> GetProperty(const std::string& name) const {
        try {
            return std::any_cast<T>(mProperties.at(name));
        } catch (...) {
            return std::nullopt;
        }
    }

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
    std::unordered_map<std::string, std::any> mProperties;
};

#endif //ENGINE3D_SRC_OBJECT_HPP

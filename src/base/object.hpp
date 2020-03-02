//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_OBJECT_HPP
#define ENGINE3D_SRC_OBJECT_HPP

#include <string>

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] const std::string &GetName() const;
    void SetName(const std::string &name);

protected:
    std::string mName;
};

#endif //ENGINE3D_SRC_OBJECT_HPP

//
// Created by Washington on 21/02/2020.
//

#include "object.hpp"

const std::string &Object::GetName() const {
    return mName;
}

void Object::SetName(const std::string &name) {
    mName = name;
}

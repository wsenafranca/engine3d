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

const std::unordered_map<std::string, std::any> &Object::GetProperties() const {
    return mProperties;
}

bool Object::HasProperty(const std::string &name) const {
    return mProperties.find(name) != mProperties.end();
}

//
// Created by Washington on 11/03/2020.
//

#include "component.hpp"
#include <graphics/node.hpp>

const std::weak_ptr<Node> &Component::GetTarget() const {
    return mTarget;
}

void Component::SetTarget(const std::weak_ptr<Node> &target) {
    mTarget = target;
}

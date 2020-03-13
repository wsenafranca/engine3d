//
// Created by Washington on 12/03/2020.
//

#include "rigidbody.hpp"

#include <utility>

RigidBody::RigidBody(std::shared_ptr<Body> body) : mBody(std::move(body)) {}

void RigidBody::Update(float dt) {
    auto target = mTarget.lock();
    target->SetPosition(mBody->GetPosition());
}

const std::shared_ptr<Body> &RigidBody::GetBody() const {
    return mBody;
}

//
// Created by Washington on 14/03/2020.
//

#include <glm/gtc/type_ptr.hpp>
#include "body.hpp"
#include "physics.hpp"

Body::~Body() {
    Physics::DestroyBody(this);
}

void Body::Rotate(const glm::quat &rotation) {
    auto rot = btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);
    auto t = mBody->getWorldTransform();
    t.setRotation(t.getRotation()*rot);
    mBody->setWorldTransform(t);
}

void Body::Translate(const glm::vec3 &translation) {
    auto pos = btVector3(translation.x, translation.y, translation.z);
    auto t = mBody->getWorldTransform();
    t.setOrigin(t.getOrigin() + pos);
    mBody->setWorldTransform(t);
}

glm::vec3 Body::GetPosition() const {
    return glm::make_vec3(&mBody->getWorldTransform().getOrigin().x());
}

void Body::SetPosition(const glm::vec3 &position) {
    auto pos = btVector3(position.x, position.y, position.z);
    auto t = mBody->getWorldTransform();
    t.setOrigin(pos);
    mBody->setWorldTransform(t);
}

glm::quat Body::GetRotation() const {
    glm::quat q(
            mBody->getWorldTransform().getRotation().w(),
            mBody->getWorldTransform().getRotation().x(),
            mBody->getWorldTransform().getRotation().y(),
            mBody->getWorldTransform().getRotation().z());
    return q;
}

void Body::SetRotation(const glm::quat &rotation) {
    auto rot = btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);
    auto t = mBody->getWorldTransform();
    t.setRotation(rot);
    mBody->setWorldTransform(t);
}

Transform Body::GetTransform() const {
    Transform transform;
    auto t = mBody->getWorldTransform();
    transform.position = glm::vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());
    transform.rotation = glm::quat(t.getRotation().w(), t.getRotation().x(), t.getRotation().y(), t.getRotation().z());
    return transform;
}

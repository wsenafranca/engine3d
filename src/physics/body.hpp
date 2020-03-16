//
// Created by Washington on 14/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_BODY_HPP
#define ENGINE3D_SRC_PHYSICS_BODY_HPP

#include <math/transform.hpp>
#include <btBulletDynamicsCommon.h>

class Body {
public:
    ~Body();

    void Rotate(const glm::quat &rotation);
    void Translate(const glm::vec3 &translation);

    [[nodiscard]] glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3 &position);

    [[nodiscard]] glm::quat GetRotation() const;
    void SetRotation(const glm::quat &rotation);

    [[nodiscard]] Transform GetTransform() const;

    friend class TPhysics;

private:
    btRigidBody *mBody{nullptr};
};

#endif //ENGINE3D_SRC_PHYSICS_BODY_HPP

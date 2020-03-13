//
// Created by Washington on 13/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_SHAPE_HPP
#define ENGINE3D_SRC_PHYSICS_SHAPE_HPP

#include <math/transform.hpp>
#include "ray.hpp"
#include "raycast.hpp"
#include "bounds.hpp"

struct ShapeMassData {
    float mass;
    glm::vec3 center;
    float I;
};

class Shape {
public:
    virtual ~Shape() = default;
    [[nodiscard]] virtual Shape *Clone() const = 0;
    [[nodiscard]] virtual bool TestPoint(const Transform &xf, const glm::vec3 &p) const = 0;
    [[nodiscard]] virtual bool RayCast(RayCastHit &output, const Ray &input, const Transform &transform) const = 0;
    virtual void ComputeBounds(Bounds &output, const Transform &transform) const = 0;
    virtual void ComputeMass() const = 0;
    float mRadius{0.0f};
};


#endif //ENGINE3D_SRC_PHYSICS_SHAPE_HPP

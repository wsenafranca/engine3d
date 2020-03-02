//
// Created by Washington on 27/02/2020.
//

#ifndef ENGINE3D_SRC_TRANSFORM_HPP
#define ENGINE3D_SRC_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    Transform() = default;
    Transform(const Transform& rhs) = default;

    inline void Translate(const glm::vec3& translation_) {
        position += translation_;
    }

    inline void Rotate(const glm::quat& rotation_) {
        rotation *= rotation_;
    }

    inline void Scale(const glm::vec3& scale_) {
        scale *= scale_;
    }

    [[nodiscard]] inline glm::mat4 ToMatrix() const {
        static const glm::mat4 I{1.0f};
        return glm::translate(I, position)*glm::mat4_cast(rotation)*glm::scale(I, scale);
    }
};

class Transformable {
public:
    virtual ~Transformable() = default;

    Transform transform;
};

#endif //ENGINE3D_SRC_TRANSFORM_HPP

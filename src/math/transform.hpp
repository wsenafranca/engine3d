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
    Transform(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale) :
            position(translation), rotation(rotation), scale(scale) {}
    Transform(const glm::mat4& matrix) {
        auto m = matrix;
        position.x = m[3][0];
        position.y = m[3][1];
        position.z = m[3][2];

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        scale.x = glm::length(glm::vec3(m[0][0], m[1][0], m[2][0]));
        scale.y = glm::length(glm::vec3(m[0][1], m[1][1], m[2][1]));
        scale.z = glm::length(glm::vec3(m[0][2], m[1][2], m[2][2]));

        if(scale.x != 0.0f) {
            m[0][0]/=scale.x;
            m[1][0]/=scale.x;
            m[2][0]/=scale.x;
        } else {
            m[0][0] = m[1][0] = m[2][0] = 0;
        }
        if(scale.y != 0.0f) {
            m[0][1]/=scale.y;
            m[1][1]/=scale.y;
            m[2][1]/=scale.y;
        } else {
            m[0][1] = m[1][1] = m[2][1] = 0;
        }
        if(scale.z != 0.0f) {
            m[0][2]/=scale.z;
            m[1][2]/=scale.z;
            m[2][2]/=scale.z;
        } else {
            m[0][2] = m[1][2] =m[2][2] = 0;
        }

        rotation = glm::quat_cast(m);
    }

    inline void Set(const Transform &rhs) {
        position = rhs.position;
        rotation = rhs.rotation;
        scale = rhs.scale;
    }

    inline void Set(const glm::vec3& translation_, const glm::quat& rotation_, const glm::vec3& scale_) {
        position = translation_;
        rotation = rotation_;
        scale = scale_;
    }

    inline void Translate(const glm::vec3& translation_) {
        position += translation_;
    }

    inline void Rotate(const glm::quat& rotation_) {
        rotation *= rotation_;
    }

    inline void Scale(const glm::vec3& scale_) {
        scale *= scale_;
    }

    inline void Lerp(const Transform& rhs, float a) {
        position = glm::mix(position, rhs.position, a);
        rotation = glm::slerp(rotation, rhs.rotation, a);
        scale = glm::mix(scale, rhs.scale, a);
    }

    [[nodiscard]] inline glm::mat4 ToMatrix() const {
        static const glm::mat4 I{1.0f};
        return glm::translate(I, position)*glm::mat4_cast(rotation)*glm::scale(I, scale);
    }

    inline bool operator==(const Transform &rhs) const {
        return position == rhs.position &&
               rotation == rhs.rotation &&
               scale == rhs.scale;
    }

    inline bool operator!=(const Transform &rhs) const {
        return !(rhs == *this);
    }
};

class Transformable {
public:
    virtual ~Transformable() = default;

    Transform transform;
};

#endif //ENGINE3D_SRC_TRANSFORM_HPP

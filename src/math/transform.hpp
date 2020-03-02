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

    inline static Transform Decompose(const glm::mat4& matrix) {
        auto m = matrix;
        Transform t;
        t.position.x = m[3][0];
        t.position.y = m[3][1];
        t.position.z = m[3][2];

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        t.scale.x = glm::length(glm::vec3(m[0][0], m[1][0], m[2][0]));
        t.scale.y = glm::length(glm::vec3(m[0][1], m[1][1], m[2][1]));
        t.scale.z = glm::length(glm::vec3(m[0][2], m[1][2], m[2][2]));

        if(t.scale.x != 0.0f) {
            m[0][0]/=t.scale.x;
            m[1][0]/=t.scale.x;
            m[2][0]/=t.scale.x;
        } else {
            m[0][0] = m[1][0] = m[2][0] = 0;
        }
        if(t.scale.y != 0.0f) {
            m[0][1]/=t.scale.y;
            m[1][1]/=t.scale.y;
            m[2][1]/=t.scale.y;
        } else {
            m[0][1] = m[1][1] = m[2][1] = 0;
        }
        if(t.scale.z != 0.0f) {
            m[0][2]/=t.scale.z;
            m[1][2]/=t.scale.z;
            m[2][2]/=t.scale.z;
        } else {
            m[0][2] = m[1][2] =m[2][2] = 0;
        }

        t.rotation = glm::quat_cast(m);

        return t;
    }
};

class Transformable {
public:
    virtual ~Transformable() = default;

    Transform transform;
};

#endif //ENGINE3D_SRC_TRANSFORM_HPP

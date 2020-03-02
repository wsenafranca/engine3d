//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_EXTENSIONS_HPP
#define ENGINE3D_SRC_EXTENSIONS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace glm {
    inline void decompose(const mat4& matrix, vec3 &translation, quat &rotation, vec3 &scale) {
        auto m = matrix;
        translation.x = m[3][0];
        translation.y = m[3][1];
        translation.z = m[3][2];

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        scale.x = glm::length(vec3(m[0][0], m[1][0], m[2][0]));
        scale.y = glm::length(vec3(m[0][1], m[1][1], m[2][1]));
        scale.z = glm::length(vec3(m[0][2], m[1][2], m[2][2]));

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

        rotation = glm::toQuat(m);
    }
}

#endif //ENGINE3D_SRC_EXTENSIONS_HPP

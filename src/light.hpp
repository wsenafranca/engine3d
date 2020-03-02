//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_LIGHT_HPP
#define ENGINE3D_SRC_LIGHT_HPP

#include <glm/glm.hpp>
#include <string>

enum LightType {
    LIGHT_TYPE_DIRECTIONAL = 0,
    LIGHT_TYPE_POINT = 1,
    LIGHT_TYPE_SPOT = 2
};

struct Light {
    std::string name;
    LightType type{LIGHT_TYPE_DIRECTIONAL};
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f};
    glm::vec3 color{1.0f};
    float constantAttenuation{1.0f};
    float linearAttenuation{0.09f};
    float quadraticAttenuation{0.032f};
    float spotInnerAngle{0.0f};
    float spotOuterAngle{0.0f};
};


#endif //ENGINE3D_SRC_LIGHT_HPP

//
// Created by Washington on 25/02/2020.
//

#include "lightsystem.hpp"

Light *
LightSystem::CreateDirectionalLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &direction) {
    auto &light = mLights[name] = std::make_unique<Light>();
    light->name = name;
    light->type = LIGHT_TYPE_DIRECTIONAL;
    light->color = color;
    light->direction = direction;
    return light.get();
}

Light *LightSystem::CreatePointLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &position) {
    auto &light = mLights[name] = std::make_unique<Light>();
    light->name = name;
    light->type = LIGHT_TYPE_POINT;
    light->color = color;
    light->position = position;
    return light.get();
}

Light *LightSystem::CreateSpotLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &direction,
                                    const glm::vec3 &position, float innerAngle, float outerAngle) {
    auto &light = mLights[name] = std::make_unique<Light>();
    light->name = name;
    light->type = LIGHT_TYPE_SPOT;
    light->color = color;
    light->direction = direction;
    light->position = position;
    light->spotInnerAngle = innerAngle;
    light->spotOuterAngle = outerAngle;
    return light.get();
}

Light *LightSystem::GetLight(const std::string &name) const {
    auto it = mLights.find(name);
    if(it != mLights.end()) {
        return it->second.get();
    }
    return nullptr;
}

void LightSystem::DestroyLight(const std::string &name) {
    auto it = mLights.find(name);
    if(it != mLights.end()) {
        mLights.erase(it);
    }
}

void LightSystem::DestroyLight(Light *light) {
    if(!light) return;
    DestroyLight(light->name);
}

const std::unordered_map<std::string, std::unique_ptr<Light>> &LightSystem::GetLights() const {
    return mLights;
}

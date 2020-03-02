//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_LIGHTSYSTEM_HPP
#define ENGINE3D_SRC_LIGHTSYSTEM_HPP

#include "texture.hpp"
#include "light.hpp"
#include <unordered_map>
#include <string>

class LightSystem {
public:
    Light* CreateDirectionalLight(const std::string& name, const glm::vec3& color, const glm::vec3& direction);
    Light* CreatePointLight(const std::string& name, const glm::vec3& color, const glm::vec3& position);
    Light* CreateSpotLight(const std::string& name, const glm::vec3& color, const glm::vec3& direction, const glm::vec3& position, float innerAngle, float outerAngle);

    [[nodiscard]] Light* GetLight(const std::string& name) const;

    void DestroyLight(const std::string& name);
    void DestroyLight(Light* light);

    const std::unordered_map<std::string, std::unique_ptr<Light>> &GetLights() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Light> > mLights;
};

#endif //ENGINE3D_SRC_LIGHTSYSTEM_HPP

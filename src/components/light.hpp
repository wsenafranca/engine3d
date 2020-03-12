//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_LIGHT_HPP
#define ENGINE3D_SRC_COMPONENTS_LIGHT_HPP

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "component.hpp"
#include <graphics/node.hpp>

enum LightType {
    LIGHT_TYPE_DIRECTIONAL = 0,
    LIGHT_TYPE_POINT = 1,
    LIGHT_TYPE_SPOT = 2
};

class Light : public Component {
public:
    explicit Light(LightType type);

    void Update(float dt) override;

    [[nodiscard]] LightType GetType() const;

    [[nodiscard]] const glm::vec3 &GetColor() const;
    void SetColor(const glm::vec3 &color);

    [[nodiscard]] float GetConstantAttenuation() const;
    void SetConstantAttenuation(float constantAttenuation);

    [[nodiscard]] float GetLinearAttenuation() const;
    void SetLinearAttenuation(float linearAttenuation);

    [[nodiscard]] float GetQuadraticAttenuation() const;
    void SetQuadraticAttenuation(float quadraticAttenuation);

    [[nodiscard]] float GetSpotInnerAngle() const;
    void SetSpotInnerAngle(float spotInnerAngle);

    [[nodiscard]] float GetSpotOuterAngle() const;
    void SetSpotOuterAngle(float spotOuterAngle);

    [[nodiscard]] const glm::vec3 &GetWorldPosition() const;
    [[nodiscard]] const glm::vec3 &GetWorldDirection() const;

private:
    LightType mType{LIGHT_TYPE_DIRECTIONAL};
    glm::vec3 mColor{1.0f};
    float mConstantAttenuation{1.0f};
    float mLinearAttenuation{0.09f};
    float mQuadraticAttenuation{0.032f};
    float mSpotInnerAngle{0.0f};
    float mSpotOuterAngle{0.0f};
    glm::vec3 mWorldPosition{1.0f};
    glm::vec3 mWorldDirection{0.0f};
};


#endif //ENGINE3D_SRC_COMPONENTS_LIGHT_HPP

//
// Created by Washington on 11/03/2020.
//

#include "light.hpp"

Light::Light(LightType type) : mType(type) {

}

void Light::Update(float) {
    auto worldMatrix = GetTarget().lock()->GetWorldMatrix();
    mWorldPosition = glm::vec3(worldMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    mWorldDirection = glm::vec3(worldMatrix*glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

LightType Light::GetType() const {
    return mType;
}

const glm::vec3 &Light::GetColor() const {
    return mColor;
}

void Light::SetColor(const glm::vec3 &color) {
    mColor = color;
}

float Light::GetConstantAttenuation() const {
    return mConstantAttenuation;
}

void Light::SetConstantAttenuation(float constantAttenuation) {
    mConstantAttenuation = constantAttenuation;
}

float Light::GetLinearAttenuation() const {
    return mLinearAttenuation;
}

void Light::SetLinearAttenuation(float linearAttenuation) {
    mLinearAttenuation = linearAttenuation;
}

float Light::GetQuadraticAttenuation() const {
    return mQuadraticAttenuation;
}

void Light::SetQuadraticAttenuation(float quadraticAttenuation) {
    mQuadraticAttenuation = quadraticAttenuation;
}

float Light::GetSpotInnerAngle() const {
    return mSpotInnerAngle;
}

void Light::SetSpotInnerAngle(float spotInnerAngle) {
    mSpotInnerAngle = spotInnerAngle;
}

float Light::GetSpotOuterAngle() const {
    return mSpotOuterAngle;
}

void Light::SetSpotOuterAngle(float spotOuterAngle) {
    mSpotOuterAngle = spotOuterAngle;
}

const glm::vec3 &Light::GetWorldPosition() const {
    return mWorldPosition;
}

const glm::vec3 &Light::GetWorldDirection() const {
    return mWorldDirection;
}

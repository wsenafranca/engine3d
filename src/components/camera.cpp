//
// Created by Washington on 24/02/2020.
//

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspect, float zNear, float zFar) :
    mFieldOfView(fov), mAspect(aspect), mZNear(zNear), mZFar(zFar) {}

void Camera::Update(float) {
    auto worldMatrix = GetTarget().lock()->GetWorldMatrix();
    mWorldPosition = glm::vec3(worldMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    glm::vec3 dir = glm::vec3(worldMatrix*glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    mViewMatrix = glm::lookAt(mWorldPosition, mWorldPosition + dir, glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4 &Camera::GetProjectionMatrix() const {
    if(mNeedRebuildProjection) {
        mNeedRebuildProjection = false;
        mProjectionMatrix = glm::perspective(mFieldOfView, mAspect, mZNear, mZFar);
    }
    return mProjectionMatrix;
}

const glm::mat4 &Camera::GetViewMatrix() const {
    return mViewMatrix;
}

const glm::vec3 &Camera::GetWorldPosition() const {
    return mWorldPosition;
}

float Camera::GetFieldOfView() const {
    return mFieldOfView;
}

void Camera::SetFieldOfView(float fieldOfView) {
    if(mFieldOfView != fieldOfView) {
        mFieldOfView = fieldOfView;
        mNeedRebuildProjection = true;
    }
}

float Camera::GetAspect() const {
    return mAspect;
}

void Camera::SetAspect(float aspect) {
    if(mAspect != aspect) {
        mAspect = aspect;
        mNeedRebuildProjection = true;
    }
}

float Camera::GetZNear() const {
    return mZNear;
}

void Camera::SetZNear(float zNear) {
    if(mZNear != zNear) {
        mZNear = zNear;
        mNeedRebuildProjection = true;
    }
}

float Camera::GetZFar() const {
    return mZFar;
}

void Camera::SetZFar(float zFar) {
    if(mZFar != zFar) {
        mZFar = zFar;
        mNeedRebuildProjection = true;
    }
}

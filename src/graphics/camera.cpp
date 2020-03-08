//
// Created by Washington on 24/02/2020.
//

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspect, float zNear, float zFar) :
    mFieldOfView(fov), mAspect(aspect), mZNear(zNear), mZFar(zFar) {}

void Camera::LookAt(const glm::vec3 &target) {
    auto dir = glm::normalize(target - position);
    pitch = std::asin(dir.y);
    yaw = std::atan2(dir.x, dir.z);
}

const glm::mat4 &Camera::GetProjectionMatrix() const {
    if(mNeedRebuildProjection) {
        mNeedRebuildProjection = false;
        mProjectionMatrix = glm::perspective(mFieldOfView, mAspect, mZNear, mZFar);
    }
    return mProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const {
    auto rot =  glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f))*
                glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    worldPosition = glm::vec3(parentMatrix*glm::vec4(position, 1.0f));
    worldPosition.y = glm::max(worldPosition.y, 0.5f);
    //worldPosition.z = glm::max(worldPosition.z, 0.0f);
    auto localCenter = rot*(glm::vec3(0.0f, 0.0f, -1.0f));
    auto worldCenter = glm::vec3(parentMatrix*glm::vec4(localCenter, 1.0f));
    auto viewMatrix = glm::lookAt(worldPosition, worldCenter, glm::vec3(0.0f, 1.0f, 0.0));
    return viewMatrix;
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

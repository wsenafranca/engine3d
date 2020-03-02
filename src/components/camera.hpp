//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_CAMERA_HPP
#define ENGINE3D_SRC_CAMERA_HPP

#include <glm/glm.hpp>
#include "../math/transform.hpp"

class Camera {
public:
    Camera(float fov, float aspect, float zNear, float zFar);

    void LookAt(const glm::vec3& target);

    [[nodiscard]] const glm::mat4 &GetProjectionMatrix() const;

    float GetFieldOfView() const;
    void SetFieldOfView(float fieldOfView);

    float GetAspect() const;
    void SetAspect(float aspect);

    float GetZNear() const;
    void SetZNear(float zNear);

    float GetZFar() const;
    void SetZFar(float zFar);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    float roll{0.0f};
    float pitch{0.0f};
    float yaw{0.0f};
    glm::vec3 position{0.0f};
    glm::vec3 restPosition{0.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
    glm::vec3 upAxis{0.0f, 1.0f, 0.0f};

    mutable glm::vec3 worldPosition{0.0f};
    glm::mat4 parentMatrix{1.0f};
private:
    mutable glm::mat4 mProjectionMatrix{1.0f};
    mutable bool mNeedRebuildProjection{true};
    float mFieldOfView, mAspect, mZNear, mZFar;
};


#endif //ENGINE3D_SRC_CAMERA_HPP

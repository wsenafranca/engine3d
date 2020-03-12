//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_CAMERA_HPP
#define ENGINE3D_SRC_COMPONENTS_CAMERA_HPP

#include "component.hpp"
#include <graphics/node.hpp>

class Camera : public Component {
public:
    Camera(float fov, float aspect, float zNear, float zFar);

    void Update(float dt) override;

    [[nodiscard]] const glm::mat4 &GetProjectionMatrix() const;
    [[nodiscard]] const glm::mat4 &GetViewMatrix() const;
    [[nodiscard]] const glm::vec3 &GetWorldPosition() const;

    float GetFieldOfView() const;
    void SetFieldOfView(float fieldOfView);

    float GetAspect() const;
    void SetAspect(float aspect);

    float GetZNear() const;
    void SetZNear(float zNear);

    float GetZFar() const;
    void SetZFar(float zFar);

private:
    glm::mat4 mViewMatrix{1.0f};
    glm::vec3 mWorldPosition{0.0f};
    mutable glm::mat4 mProjectionMatrix{1.0f};
    mutable bool mNeedRebuildProjection{true};
    float mFieldOfView, mAspect, mZNear, mZFar;
};


#endif //ENGINE3D_SRC_COMPONENTS_CAMERA_HPP

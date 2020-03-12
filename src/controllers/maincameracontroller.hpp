//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_CONTROLLERS_MAINCAMERACONTROLLER_HPP
#define ENGINE3D_SRC_CONTROLLERS_MAINCAMERACONTROLLER_HPP

#include "controller.hpp"
#include <graphics/node.hpp>
#include <components/camera.hpp>

class MainCameraController : public Controller {
public:
    [[nodiscard]] const std::weak_ptr<Node> &GetCameraTarget() const;

    void SetCameraTarget(const std::weak_ptr<Node> &cameraTarget);

protected:
    void OnInit(Node *target) override;
    void OnUpdate(Node *target, float dt) override;

private:
    std::weak_ptr<Node> mCameraTarget;
    glm::vec3 mOffset{0.0f};
    glm::quat mRestRot{};
    std::weak_ptr<Camera> mCamera;
};


#endif //ENGINE3D_SRC_CONTROLLERS_MAINCAMERACONTROLLER_HPP

//
// Created by Washington on 12/03/2020.
//

#include "maincameracontroller.hpp"
#include <sys/input.hpp>

void MainCameraController::OnInit(Node *target) {
    mOffset = target->GetPosition();
    mRestRot = target->GetRotation();
    mCamera = target->GetComponent<Camera>();
}

void MainCameraController::OnUpdate(Node *target, float dt) {
    auto camTarget = mCameraTarget.lock();

    if(Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        auto rot =  glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_X), glm::vec3(0.0f, 1.0f, 0.0f))*
                    glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_Y), glm::vec3(1.0f, 0.0f, 0.0f));
        target->SetPosition(rot*target->GetPosition());
    } else {
        target->SetPosition(camTarget->GetPosition() + mOffset);
    }

    auto lookAt = glm::quatLookAt(
            glm::normalize(camTarget->GetPosition() - target->GetPosition()),
            glm::vec3(0.0f, 1.0f, 0.0f));
    target->SetRotation(lookAt);

    auto camera = mCamera.lock();
    float fov = camera->GetFieldOfView();
    fov = glm::clamp(fov - Input::GetAxis(INPUT_AXIS_SCROLL_WHEEL)*dt,
                     glm::radians(5.0f),
                     glm::radians(80.0f));
    camera->SetFieldOfView(fov);
}

const std::weak_ptr<Node> &MainCameraController::GetCameraTarget() const {
    return mCameraTarget;
}

void MainCameraController::SetCameraTarget(const std::weak_ptr<Node> &cameraTarget) {
    mCameraTarget = cameraTarget;
}

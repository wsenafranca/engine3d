//
// Created by Washington on 12/03/2020.
//

#include "playercontroller.hpp"
#include <sys/input.hpp>

void PlayerController::OnInit(Node *target) {
    mAnimator = target->GetComponent<Animator>();
}

void PlayerController::OnUpdate(Node *target, float dt) {
    moveDirection = glm::vec3(Input::GetAxis(INPUT_AXIS_X), 0.0f, Input::GetAxis(INPUT_AXIS_Y));

    float mag = glm::length(moveDirection) > 0.0f;
    if(mag > 1.0f) {
        moveDirection = glm::normalize(moveDirection);
    }
    if(mag > 0.0f) {
        float angle = glm::atan(moveDirection.x, moveDirection.z);
        target->SetRotation(glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f)));
        target->SetPosition(target->GetPosition() + moveDirection*6.0f*dt);
    }

    auto animator = mAnimator.lock();
    animator->SetProperty("isMoving", mag > 0.0f);
}

//
// Created by Washington on 12/03/2020.
//

#include "playercontroller.hpp"
#include <sys/input.hpp>
#include <physics/physics.hpp>

void PlayerController::OnInit(Node *target) {
    mAnimator = target->GetComponent<Animator>();
    mCharacterMovement = target->GetComponent<CharacterMovement>();
}

void PlayerController::OnUpdate(Node *target, float dt) {
    auto characterMovement = mCharacterMovement.lock();

    if(characterMovement->GetCharacter()->OnGround()) {
        moveDirection = glm::vec3(Input::GetAxis(INPUT_AXIS_X), 0.0f, Input::GetAxis(INPUT_AXIS_Y));
        float len = glm::length(moveDirection) > 0.0f;
        if(len > 0.0f) {
            float angle = glm::atan(moveDirection.x, moveDirection.z);
            characterMovement->GetCharacter()->SetRotation(glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        if(Input::GetKeyDown(KEY_SPACE)) {
            characterMovement->GetCharacter()->Jump(12.0f);
        }
    }

    characterMovement->GetCharacter()->Move(moveDirection*6.0f*Physics::FIXED_DELTA_TIME);

    auto animator = mAnimator.lock();
    animator->SetProperty("moving", characterMovement->GetCharacter()->IsWalking());
    animator->SetProperty("jumping", characterMovement->GetCharacter()->IsJumping());
    animator->SetProperty("falling", !characterMovement->GetCharacter()->OnGround());
    animator->SetProperty("grounded", characterMovement->GetCharacter()->OnGround());
}

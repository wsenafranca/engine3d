//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_CONTROLLERS_PLAYERCONTROLLER_HPP
#define ENGINE3D_SRC_CONTROLLERS_PLAYERCONTROLLER_HPP

#include "controller.hpp"
#include <graphics/node.hpp>
#include <components/animator.hpp>
#include <components/charactermovement.hpp>

class PlayerController : public Controller {
protected:
    void OnInit(Node *target) override;
    void OnUpdate(Node *target, float dt) override;

private:
    std::weak_ptr<Animator> mAnimator;
    std::weak_ptr<CharacterMovement> mCharacterMovement;
    glm::vec3 moveDirection{0.0f, 0.0f, 0.0f};
};


#endif //ENGINE3D_SRC_CONTROLLERS_PLAYERCONTROLLER_HPP

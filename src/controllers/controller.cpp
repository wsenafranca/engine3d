//
// Created by Washington on 12/03/2020.
//

#include "controller.hpp"

Controller::Controller() {
    mState = std::shared_ptr<State>(new AwakeState());
}

void Controller::Update(float dt) {
    mState->OnTick(this, dt);
}

const std::weak_ptr<Node> &Controller::GetTarget() const {
    return mTarget;
}

void Controller::SetTarget(const std::weak_ptr<Node> &target) {
    mTarget = target;
}

void Controller::OnInit(Node*) {}

void Controller::OnUpdate(Node*, float) {}

void Controller::AwakeState::OnTick(Controller *controller, float dt) {
    controller->OnInit(controller->mTarget.lock().get());
    controller->mState = std::shared_ptr<State>(new AliveState());
}

void Controller::AliveState::OnTick(Controller *controller, float dt) {
    controller->OnUpdate(controller->mTarget.lock().get(), dt);
}

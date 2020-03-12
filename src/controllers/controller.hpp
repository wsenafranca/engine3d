//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_CONTROLLERS_CONTROLLER_HPP
#define ENGINE3D_SRC_CONTROLLERS_CONTROLLER_HPP

#include <memory>

class Node;

class Controller {
public:
    Controller();
    virtual ~Controller() = default;

    void Update(float dt);

    [[nodiscard]] const std::weak_ptr<Node> &GetTarget() const;
    void SetTarget(const std::weak_ptr<Node> &target);

protected:
    virtual void OnInit(Node* target);
    virtual void OnUpdate(Node* target, float dt);

    class State {
    public:
        virtual void OnTick(Controller *controller, float dt) = 0;
    };

    class AwakeState : public State {
    public:
        void OnTick(Controller *controller, float dt) override;
    };

    class AliveState : public State {
    public:
        void OnTick(Controller *controller, float dt) override;
    };

protected:
    std::weak_ptr<Node> mTarget;

private:
    std::shared_ptr<State> mState;
};


#endif //ENGINE3D_SRC_CONTROLLERS_CONTROLLER_HPP

//
// Created by Washington on 07/03/2020.
//

#ifndef ENGINE3D_SRC_CONTROLLERS_ANIMCONTROLLER_HPP
#define ENGINE3D_SRC_CONTROLLERS_ANIMCONTROLLER_HPP

#include "basecontroller.hpp"
#include <graphics/model.hpp>
#include <graphics/modelinstance.hpp>
#include "animations/animstate.hpp"

class AnimController : public BaseController<ModelInstance> {
public:
    explicit AnimController(const std::string &name);

    void Update(float dt) override;

    AnimState *GetAnimation(const std::string& name) const;
    AnimState *AddAnimation(const std::string& name, Animation* animation);

    void SetTarget(const std::shared_ptr<ModelInstance> &target) override;

protected:
    void Apply(Animation* animation, float time);
    void Blending(Animation* animation, float time, float a);

private:
    std::unordered_map< std::string, Transform > mTransformMap;
    std::unordered_map< std::string, std::unique_ptr<AnimState> > mStateMap;
    AnimState* mCurrentState{nullptr};
    mutable AnimState* mPrevState{nullptr};
    float mTimeState{0.0f};
    bool mInBetween{false};
};


#endif //ENGINE3D_SRC_CONTROLLERS_ANIMCONTROLLER_HPP

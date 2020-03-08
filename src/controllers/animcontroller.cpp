//
// Created by Washington on 07/03/2020.
//

#include "animcontroller.hpp"

AnimController::AnimController(const std::shared_ptr<ModelInstance> &target) : BaseController(target) {
    mTransformMap = target->GetNodeTransformMap();
}

void AnimController::Update(float dt) {
    mTimeState += dt;
    mCurrentState->isFinished = false;
    if(mCurrentState->animation->IsFinishedAt(mTimeState)) {
        if(mCurrentState->playMode == ANIM_PLAY_MODE_NORMAL) {
            mCurrentState->isFinished = true;
        } else if(mCurrentState->playMode == ANIM_PLAY_MODE_LOOP) {
            mTimeState = 0.0f;
        }
    }

    for(const auto& t : mCurrentState->transitions) {
        auto frame = mCurrentState->animation->GetFrameAt(mTimeState);
        if(mCurrentState->frameCallback.find(frame) != mCurrentState->frameCallback.end()) {
            mCurrentState->frameCallback[frame]();
        }
        if(t.f && t.f(this, mCurrentState, frame)) {
            mPrevState = mCurrentState;
            mCurrentState = mStateMap[t.target].get();
            mTimeState = 0.0f;
            mInBetween = true;
            break;
        }
    }

    if(mInBetween) {
        float a = std::min(1.0f, mTimeState/0.5f);
        mInBetween = a < 1.0f;
        Blending(mCurrentState->animation.get(), mTimeState, a);
    } else {
        Apply(mCurrentState->animation.get(), mTimeState);
    }

    for(auto& it : mTransformMap) {
        mTarget->SetNodeTransform(it.first, it.second.ToMatrix());
    }
}

void AnimController::Apply(Animation* animation, float time) {
    for(const auto& p : animation->GetChannels()) {
        auto it = mTransformMap.find(p.first);
        if(it == mTransformMap.end()) {
            mTransformMap[p.first] = mTarget->GetNodeTransform(p.first);
        } else {
            it->second.Set(mCurrentState->animation->GetTransformAt(p.first, time));
        }
    }
}

void AnimController::Blending(Animation* animation, float time, float a) {
    for(const auto& p : animation->GetChannels()) {
        auto transform = animation->GetTransformAt(p.first, time);
        auto it = mTransformMap.find(p.first);
        if(it == mTransformMap.end()) {
            if(a > 0.999f) {
                it->second.Set(transform);
            } else {
                it->second.Lerp(transform, a);
            }
        } else {
            if(a > 0.999f) {
                mTransformMap[p.first] = transform;
            } else {
                Transform restTransform = mTarget->GetNodeTransform(p.first);
                restTransform.Lerp(transform, a);
                mTransformMap[p.first] = restTransform;
            }
        }
    }
}

AnimState *AnimController::GetAnimation(const std::string &name) const {
    return mStateMap.at(name).get();
}

AnimState *AnimController::AddAnimation(const std::string &name, Animation *animation) {
    auto& state = mStateMap[name] = std::make_unique<AnimState>();
    state->animation.reset(animation);
    if(mCurrentState == nullptr) {
        mCurrentState = state.get();
    }
    return state.get();
}

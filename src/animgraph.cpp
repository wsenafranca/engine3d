//
// Created by Washington on 26/02/2020.
//

#include "animgraph.hpp"

void AnimGraph::Update(float dt) {
    if(!mCurrentState) SetCurrentState(mRoot);
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
            SetCurrentState(t.target);
            break;
        }
    }
}

AnimState *AnimGraph::AddAnimation(const std::string &name, Animation *animation) {
    auto& state = mGraph[name] = std::make_unique<AnimState>();
    state->animation.reset(animation);
    if(mRoot.empty()) mRoot = name;
    return state.get();
}

AnimState *AnimGraph::GetState(const std::string &name) const {
    return mGraph.at(name).get();
}

AnimState *AnimGraph::GetCurrentState() const {
    return mCurrentState;
}

void AnimGraph::SetCurrentState(const std::string &name) {
    if(mCurrentState && mCurrentState->endCallback) mCurrentState->endCallback();
    mPrevState = mCurrentState;
    mCurrentState = mGraph[name].get();
    if(mCurrentState->startCallback) mCurrentState->startCallback();
    mTimeState = 0.0f;
}

Transform AnimGraph::GetCurrentAnimationTransform(const std::string &channel) const {
    const auto& currentAnimation = mCurrentState->animation;
    float time = mTimeState*currentAnimation->GetTicksPerSecond();
    float between = 0.8f;
    if(time < between && mPrevState) {
        const auto& lastAnimation = mPrevState->animation;
        const auto& lastChannel = lastAnimation->GetChannel(channel);
        const auto& currentChannel = currentAnimation->GetChannel(channel);

        Transform result;
        if(!currentChannel.translationKeys.empty()) {
            if(lastChannel.translationKeys.empty() || currentChannel.translationKeys.size() == 1) {
                result.position = currentChannel.translationKeys.front().value;
            } else {
                const auto& lastFrame = lastChannel.translationKeys.back();
                const auto& currentFrame = currentChannel.translationKeys[0];
                float dt = between - time;
                float a = glm::clamp(time/dt, 0.0f, 1.0f);
                result.position = glm::mix(lastFrame.value, currentFrame.value, a);
            }
        }

        if(!currentChannel.rotationKeys.empty()) {
            if(lastChannel.rotationKeys.empty() || currentChannel.rotationKeys.size() == 1) {
                result.rotation = currentChannel.rotationKeys.front().value;
            } else {
                const auto& lastFrame = lastChannel.rotationKeys.back();
                const auto& currentFrame = currentChannel.rotationKeys[0];
                float dt = between - time;
                float a = glm::clamp(time/dt, 0.0f, 1.0f);
                result.rotation = glm::normalize(glm::slerp(lastFrame.value, currentFrame.value, a));
            }
        }

        if(!currentChannel.scaleKeys.empty()) {
            if(lastChannel.scaleKeys.empty() || currentChannel.scaleKeys.size() == 1) {
                result.scale = currentChannel.scaleKeys.front().value;
            } else {
                const auto& lastFrame = lastChannel.scaleKeys.back();
                const auto& currentFrame = currentChannel.scaleKeys[0];
                float dt = between - time;
                float a = glm::clamp(time/dt, 0.0f, 1.0f);
                result.scale = glm::mix(lastFrame.value, currentFrame.value, a);
            }
        }

        return result;
    }

    mPrevState = nullptr;
    return mCurrentState->animation->GetTransformAt(channel, mTimeState);
}

Animation *AnimGraph::GetCurrentAnimation() const {
    return mCurrentState->animation.get();
}

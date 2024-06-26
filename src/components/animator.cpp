//
// Created by Washington on 07/03/2020.
//

#include "animator.hpp"
#include <lua/luamanager.hpp>
#include <algorithm>

std::shared_ptr<Animator> Animator::Load(const std::string &tableName) {
    auto animController = std::make_shared<Animator>();
    try {
        auto controller = (*LuaManager::GetLuaState())["Animators"][tableName].get<sol::table>();

        for(auto p : controller) {
            const sol::object& key = p.first;
            const sol::object& value = p.second;
            const auto& stateName = key.as<std::string>();
            if(stateName == "initialState") continue;

            const auto& stateTable = value.as<sol::table>();
            std::string animFile = stateTable["file"];
            auto animation = Animation::Load(animFile);
            auto state = animController->AddAnimation(stateName, animation);
            state->playMode = stateTable.get_or("playMode", 0);
            auto transitionsTable = stateTable.get<sol::table>("transitions");
            for(auto p2 : transitionsTable) {
                AnimTransition transition;
                const sol::object& key2 = p2.first;
                const sol::object& value2 = p2.second;
                transition.target = key2.as<std::string>();
                auto luaTransitionFunction = value2.as<sol::function>();
                transition.f = [luaTransitionFunction](ANIM_STATE_TRANSITION_FUNC) {
                    return luaTransitionFunction(graph, state, frame);
                };
                state->transitions.push_back(transition);
            }
            auto frameCallbacks = stateTable["frameCallbacks"];
            if(frameCallbacks != sol::nil) {
                auto frameCallbacksTable = frameCallbacks.get<sol::table>();
                for(size_t i = 1; i <= frameCallbacksTable.size(); i++) {
                    auto frameTable = frameCallbacksTable.get<sol::table>(i);
                    int frame = frameTable.get<int>("frame");
                    auto callback = frameTable.get<sol::function>("callback");
                    state->frameCallback[frame] = [callback](ANIM_STATE_TRANSITION_FUNC) {
                        callback(graph, state, frame);
                    };
                }
            }
            auto endCallback = stateTable["endCallback"];
            if(endCallback != sol::nil) {
                auto callback = endCallback.get<sol::function>();
                state->endCallback = [callback](ANIM_STATE_TRANSITION_FUNC) {
                    callback(graph, state, frame);
                };
            }
        }

        animController->mCurrentState = animController->mStateMap[controller.get<std::string>("initialState")].get();
    }catch (...) {
        throw std::runtime_error("Error loading Animator " + tableName);
    }

    return animController;
}

void Animator::Update(float dt) {
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
            mCurrentState->frameCallback[frame](this, mCurrentState, frame);
        }
        if(t.f && t.f(this, mCurrentState, frame)) {
            if(mCurrentState->endCallback) mCurrentState->endCallback(this, mCurrentState, frame);
            mCurrentState = mStateMap[t.target].get();
            mTimeState = 0.0f;
            mInBetween = true;
            break;
        }
    }

    if(mInBetween) {
        float a = (std::min)(1.0f, mTimeState/0.5f);
        mInBetween = a < 1.0f;
        Blending(mCurrentState->animation.get(), mTimeState, a);
    } else {
        Apply(mCurrentState->animation.get(), mTimeState);
    }

    for(auto& it : mTransformMap) {
        mNodeMap[it.first]->SetLocalTransform(it.second);
    }
}

void Animator::Apply(Animation* animation, float time) {
    for(const auto& p : animation->GetChannels()) {
        auto it = mTransformMap.find(p.first);
        if(it == mTransformMap.end()) {
            mTransformMap[p.first] = mNodeMap[p.first]->GetLocalTransform();
        } else {
            it->second.Set(mCurrentState->animation->GetTransformAt(p.first, time));
        }
    }
}

void Animator::Blending(Animation* animation, float time, float a) {
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
                Transform restTransform = mNodeMap[p.first]->GetLocalTransform();
                restTransform.Lerp(transform, a);
                mTransformMap[p.first] = restTransform;
            }
        }
    }
}

const std::shared_ptr<AnimState> &Animator::GetAnimation(const std::string &name) const {
    return mStateMap.at(name);
}

std::shared_ptr<AnimState> Animator::AddAnimation(const std::string &name, const std::shared_ptr<Animation> &animation) {
    auto state = std::make_shared<AnimState>();
    mStateMap[name] = state;
    state->animation = animation;
    if(mCurrentState == nullptr) {
        mCurrentState = state.get();
    }
    return state;
}

void Animator::SetTarget(const std::weak_ptr<Node> &target) {
    Component::SetTarget(target);
    mTransformMap.clear();
    target.lock()->GetChildren().front()->Apply([this](Node *node) {
        mTransformMap[node->GetName()] = node->GetLocalTransform();
        mNodeMap[node->GetName()] = node;
    });
}

const std::unordered_map<std::string, std::any> &Animator::GetProperties() const {
    return mProperties;
}

bool Animator::HasProperty(const std::string &name) const {
    return mProperties.find(name) != mProperties.end();
}

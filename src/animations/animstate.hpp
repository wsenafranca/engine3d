//
// Created by Washington on 26/02/2020.
//

#ifndef ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP
#define ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP

#include <unordered_map>
#include "animation.hpp"
#include <functional>
#include <any>

#define ANIM_STATE_TRANSITION_FUNC AnimController* graph, AnimState* state, uint32_t frame

class AnimController;
struct AnimState;

enum AnimPlayMode {
    ANIM_PLAY_MODE_NORMAL = 0,
    ANIM_PLAY_MODE_REVERSED = 1,
    ANIM_PLAY_MODE_LOOP = 2,
    ANIM_PLAY_MODE_LOOP_REVERSED = 3
};

typedef std::function<bool(AnimController*, AnimState*, uint32_t)> AnimTransitionFunction;

struct AnimTransition {
    std::string target;
    AnimTransitionFunction f;
};

struct AnimState {
    std::unique_ptr<Animation> animation;
    std::list<AnimTransition> transitions;
    char playMode{1};
    bool isFinished{false};
    std::function<void()> startCallback;
    std::function<void()> endCallback;
    std::unordered_map<uint32_t, std::function<void()> > frameCallback;
};

#endif //ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP

//
// Created by Washington on 26/02/2020.
//

#ifndef ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP
#define ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP

#include <unordered_map>
#include "animation.hpp"
#include <functional>
#include <any>

#define ANIM_STATE_TRANSITION_FUNC Animator* graph, AnimState* state, uint32_t frame

class Animator;
struct AnimState;

enum AnimPlayMode {
    ANIM_PLAY_MODE_NORMAL = 0,
    ANIM_PLAY_MODE_REVERSED = 1,
    ANIM_PLAY_MODE_LOOP = 2,
    ANIM_PLAY_MODE_LOOP_REVERSED = 3
};

typedef std::function<bool(Animator*, AnimState*, uint32_t)> AnimTransitionFunction;

struct AnimTransition {
    std::string target;
    AnimTransitionFunction f;
};

struct AnimState {
    std::shared_ptr<Animation> animation;
    std::list<AnimTransition> transitions;
    char playMode{1};
    bool isFinished{false};
    std::function<void(Animator*, AnimState*, uint32_t)> endCallback;
    std::unordered_map<uint32_t, std::function<void(Animator*, AnimState*, uint32_t)> > frameCallback;
};

#endif //ENGINE3D_SRC_ANIMATIONS_ANIMSTATE_HPP

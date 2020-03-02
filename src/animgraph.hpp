//
// Created by Washington on 26/02/2020.
//

#ifndef ENGINE3D_SRC_ANIMGRAPH_HPP
#define ENGINE3D_SRC_ANIMGRAPH_HPP

#include <unordered_map>
#include "animation.hpp"
#include <functional>
#include <any>

#define ANIM_STATE_TRANSITION_FUNC AnimGraph* graph, AnimState* state, uint32_t frame

class AnimGraph;
struct AnimState;

enum AnimPlayMode {
    ANIM_PLAY_MODE_NORMAL = 0,
    ANIM_PLAY_MODE_REVERSED = 1,
    ANIM_PLAY_MODE_LOOP = 2,
    ANIM_PLAY_MODE_LOOP_REVERSED = 3
};

typedef std::function<bool(AnimGraph*, AnimState*, uint32_t)> AnimTransitionFunction;

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

class AnimGraph {
public:
    void Update(float dt);

    AnimState* AddAnimation(const std::string& name, Animation* animation);
    [[nodiscard]] AnimState* GetState(const std::string& name) const;
    [[nodiscard]] AnimState* GetCurrentState() const;
    void SetCurrentState(const std::string& name);
    [[nodiscard]] Transform GetCurrentAnimationTransform(const std::string &channel) const;
    [[nodiscard]] Animation* GetCurrentAnimation() const;

    template<class T>
    void SetProperty(const std::string& name, const T& value) {
        mProperties[name] = value;
    }

    template<class T>
    T GetProperty(const std::string& name, const T& fallback) {
        auto it = mProperties.find(name);
        if(it != mProperties.end()) {
            return std::any_cast<T>(it->second);
        }

        return fallback;
    }
private:
    std::string mRoot;
    AnimState* mCurrentState{nullptr};
    mutable AnimState* mPrevState{nullptr};
    std::unordered_map<std::string, std::unique_ptr<AnimState> > mGraph;
    float mTimeState{0.0f};
    std::unordered_map<std::string, std::any> mProperties;
};

#endif //ENGINE3D_SRC_ANIMGRAPH_HPP

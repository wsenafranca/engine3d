//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_ANIMATIONS_ANIMATION_HPP
#define ENGINE3D_SRC_ANIMATIONS_ANIMATION_HPP

#include <glm/glm.hpp>
#include <string>
#include "graphics/node.hpp"
#include "math/transform.hpp"

template<class T>
struct AnimationFrame {
    T value;
    float time;
};

struct AnimationChannel {
    std::vector< AnimationFrame<glm::vec3> > translationKeys;
    std::vector< AnimationFrame<glm::quat> > rotationKeys;
    std::vector< AnimationFrame<glm::vec3> > scaleKeys;
};

class Animation : public Object {
public:
    static std::shared_ptr<Animation> Load(const filesystem::path& file);

    Animation();

    [[nodiscard]] bool IsFinishedAt(float time) const;
    [[nodiscard]] uint32_t GetFrameAt(float time) const;
    [[nodiscard]] Transform GetTransformAt(const std::string& target, float time) const;

    void AddChannel(const std::string& target, const AnimationChannel& channel);
    [[nodiscard]] bool HasChannel(const std::string& target) const;
    [[nodiscard]] const AnimationChannel& GetChannel(const std::string& target) const;
    [[nodiscard]] const std::unordered_map<std::string, AnimationChannel> &GetChannels() const;

    [[nodiscard]] float GetDuration() const;
    void SetDuration(float duration);

    [[nodiscard]] float GetTicksPerSecond() const;
    void SetTicksPerSecond(float ticksPerSecond);

protected:
    [[nodiscard]] glm::vec3 GetTranslationAt(const AnimationChannel& channel, float time) const;
    [[nodiscard]] glm::quat GetRotationAt(const AnimationChannel& channel, float time) const;
    [[nodiscard]] glm::vec3 GetScaleAt(const AnimationChannel& channel, float time) const;

private:
    std::unordered_map<std::string, AnimationChannel> mChannels;
    float mDuration;
    float mTicksPerSecond;
};

#endif //ENGINE3D_SRC_ANIMATIONS_ANIMATION_HPP

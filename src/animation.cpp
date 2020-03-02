//
// Created by Washington on 25/02/2020.
//

#include "animation.hpp"
#include "internal/importer.hpp"

Animation::Animation() : mDuration(0), mTicksPerSecond(30) {

}

Animation::Animation(const filesystem::path &file) : mDuration(0), mTicksPerSecond(30) {
    Importer::LoadAnimation(this, file);
}

bool Animation::IsFinishedAt(float time) const {
    return time*mTicksPerSecond > mDuration - glm::epsilon<float>();
}

Transform Animation::GetTransformAt(const std::string& target, float time) const {
    float t = std::clamp(time*mTicksPerSecond, 0.0f, mDuration);
    auto channel = GetChannel(target);
    return Transform{
        GetTranslationAt(channel, t),
        GetRotationAt(channel, t),
        GetScaleAt(channel, t)};
}

glm::vec3 Animation::GetTranslationAt(const AnimationChannel& channel, float time) const {
    if(channel.translationKeys.empty()) return glm::vec3(0.0f);
    if(channel.translationKeys.size() == 1) return channel.translationKeys.front().value;

    uint32_t lastIndex;
    for(lastIndex = 0; lastIndex < channel.translationKeys.size()-1; lastIndex++) {
        if(time < channel.translationKeys[lastIndex + 1].time) {
            break;
        }
    }

    uint32_t nextIndex = lastIndex + 1;
    float dt = channel.translationKeys[nextIndex].time - channel.translationKeys[lastIndex].time;
    float a = (time - channel.translationKeys[lastIndex].time)/dt;
    a = glm::clamp(a, 0.0f, 1.0f);
    const auto& start = channel.translationKeys[lastIndex].value;
    const auto& end = channel.translationKeys[nextIndex].value;
    return glm::mix(start, end, a);
}

glm::quat Animation::GetRotationAt(const AnimationChannel& channel, float time) const {
    if(channel.rotationKeys.empty()) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if(channel.rotationKeys.size() == 1) return channel.rotationKeys.front().value;

    uint32_t lastIndex;
    for(lastIndex = 0; lastIndex < channel.rotationKeys.size()-1; lastIndex++) {
        if(time < channel.rotationKeys[lastIndex + 1].time) {
            break;
        }
    }

    uint32_t nextIndex = lastIndex + 1;
    float dt = channel.rotationKeys[nextIndex].time - channel.rotationKeys[lastIndex].time;
    float a = (time - channel.rotationKeys[lastIndex].time)/dt;
    a = glm::clamp(a, 0.0f, 1.0f);
    const auto& start = channel.rotationKeys[lastIndex].value;
    const auto& end = channel.rotationKeys[nextIndex].value;
    return glm::normalize(glm::slerp(start, end, a));
}

glm::vec3 Animation::GetScaleAt(const AnimationChannel& channel, float time) const {
    if(channel.scaleKeys.empty()) return glm::vec3(1.0f);
    if(channel.scaleKeys.size() == 1) return channel.scaleKeys.front().value;

    if(channel.scaleKeys.empty()) return glm::vec3(0.0f);
    if(channel.scaleKeys.size() == 1) return channel.scaleKeys.front().value;

    uint32_t lastIndex;
    for(lastIndex = 0; lastIndex < channel.scaleKeys.size()-1; lastIndex++) {
        if(time < channel.scaleKeys[lastIndex + 1].time) {
            break;
        }
    }

    uint32_t nextIndex = lastIndex + 1;
    float dt = channel.scaleKeys[nextIndex].time - channel.scaleKeys[lastIndex].time;
    float a = (time - channel.scaleKeys[lastIndex].time)/dt;
    a = glm::clamp(a, 0.0f, 1.0f);
    const auto& start = channel.scaleKeys[lastIndex].value;
    const auto& end = channel.scaleKeys[nextIndex].value;
    return glm::mix(start, end, a);
}

void Animation::AddChannel(const std::string& target, const AnimationChannel &channel) {
    mChannels[target] = channel;
}

bool Animation::HasChannel(const std::string &target) const {
    return mChannels.find(target) != mChannels.end();
}

const AnimationChannel &Animation::GetChannel(const std::string &target) const {
    return mChannels.at(target);
}

float Animation::GetDuration() const {
    return mDuration;
}

void Animation::SetDuration(float duration) {
    mDuration = duration;
}

float Animation::GetTicksPerSecond() const {
    return mTicksPerSecond;
}

void Animation::SetTicksPerSecond(float ticksPerSecond) {
    mTicksPerSecond = ticksPerSecond;
}

uint32_t Animation::GetFrameAt(float time) const {
    return (uint32_t)truncf(glm::clamp(time*mTicksPerSecond, 0.f, mDuration));
}

const std::unordered_map<std::string, AnimationChannel> &Animation::GetChannels() const {
    return mChannels;
}

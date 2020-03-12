//
// Created by Washington on 07/03/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_ANIMATOR_HPP
#define ENGINE3D_SRC_COMPONENTS_ANIMATOR_HPP

#include "components/component.hpp"
#include "animations/animstate.hpp"
#include <graphics/node.hpp>
#include <any>
#include <optional>

class Animator : public Component {
public:
    static std::shared_ptr<Animator> Load(const std::string &tableName);

    void Update(float dt) override;

    [[nodiscard]] const std::shared_ptr<AnimState> &GetAnimation(const std::string& name) const;
    std::shared_ptr<AnimState> AddAnimation(const std::string& name, const std::shared_ptr<Animation> &animation);

    void SetTarget(const std::weak_ptr<Node> &target) override;

    [[nodiscard]] const std::unordered_map<std::string, std::any> &GetProperties() const;
    [[nodiscard]] bool HasProperty(const std::string& name) const;

    template<class T>
    void SetProperty(const std::string& name, const T& value) {
        mProperties[name] = value;
    }

    template<class T>
    [[nodiscard]] std::optional<T> GetProperty(const std::string& name) const {
        try {
            return std::any_cast<T>(mProperties.at(name));
        } catch (...) {
            return std::nullopt;
        }
    }

protected:
    void Apply(Animation* animation, float time);
    void Blending(Animation* animation, float time, float a);

private:
    std::unordered_map< std::string, Transform > mTransformMap;
    std::unordered_map <std::string, Node* > mNodeMap;
    std::unordered_map< std::string, std::shared_ptr<AnimState> > mStateMap;
    std::unordered_map<std::string, std::any> mProperties;
    AnimState* mCurrentState{nullptr};
    float mTimeState{0.0f};
    bool mInBetween{false};
};


#endif //ENGINE3D_SRC_COMPONENTS_ANIMATOR_HPP

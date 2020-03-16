//
// Created by Washington on 15/03/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_CHARACTERMOVEMENT_HPP
#define ENGINE3D_SRC_COMPONENTS_CHARACTERMOVEMENT_HPP

#include "component.hpp"
#include <graphics/node.hpp>
#include <physics/character.hpp>

class CharacterMovement : public Component {
public:
    static std::shared_ptr<CharacterMovement> Create(Character *character);

    void Update(float dt) override;

    [[nodiscard]] Character *GetCharacter() const;

private:
    Character *mCharacter;
};


#endif //ENGINE3D_SRC_COMPONENTS_CHARACTERMOVEMENT_HPP

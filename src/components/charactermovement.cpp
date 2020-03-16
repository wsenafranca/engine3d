//
// Created by Washington on 15/03/2020.
//

#include "charactermovement.hpp"

std::shared_ptr<CharacterMovement> CharacterMovement::Create(Character *character) {
    auto characterMovement = std::make_shared<CharacterMovement>();
    characterMovement->mCharacter = character;
    return characterMovement;
}

void CharacterMovement::Update(float dt) {
    auto target = mTarget.lock();
    target->SetLocalTransform(mCharacter->GetTransform());
}

Character *CharacterMovement::GetCharacter() const {
    return mCharacter;
}

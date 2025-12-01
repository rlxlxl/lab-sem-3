#pragma once
#include "CharacterFactory.h"
#include "Warrior.h"
#include "Mage.h"
#include <memory>

class WaterCharFactory : public CharacterFactory {
public:
    std::unique_ptr<Character> createWarrior() const override;
    std::unique_ptr<Character> createMage() const override;
    std::unique_ptr<Character> createArcher() const override;
};

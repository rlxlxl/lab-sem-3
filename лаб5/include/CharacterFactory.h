#pragma once
#include "Character.h"
#include <memory>

class CharacterFactory {
public:
    virtual ~CharacterFactory() = default;
    virtual std::unique_ptr<Character> createWarrior() const = 0;
    virtual std::unique_ptr<Character> createMage() const = 0;
    virtual std::unique_ptr<Character> createArcher() const = 0; 
};

#pragma once
#include "Character.h"

class Warrior : public Character {
    int strength;
public:
    Warrior(const std::string& elem, int str, const std::vector<std::string>& tr);
    void info() const override;
};

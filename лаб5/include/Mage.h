#pragma once
#include "Character.h"

class Mage : public Character {
    int mana;
public:
    Mage(const std::string& elem, int m, const std::vector<std::string>& tr);
    void info() const override;
};

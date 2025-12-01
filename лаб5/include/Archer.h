#pragma once
#include "Character.h"

class Archer : public Character {
    int agility; // ловкость
public:
    Archer(const std::string& elem, int agi, const std::vector<std::string>& tr);
    void info() const override;
};

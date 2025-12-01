#include "Archer.h"
#include <iostream>

Archer::Archer(const std::string& elem, int agi, const std::vector<std::string>& tr)
    : agility(agi)
{
    element = elem;
    traits = tr;
}

void Archer::info() const {
    std::cout << "Archer | Element: " << element << ", Agility: " << agility << "\nTraits: ";
    for (const auto& t : traits) std::cout << t << " ";
    std::cout << std::endl;
}

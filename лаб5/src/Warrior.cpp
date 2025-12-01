#include "Warrior.h"

Warrior::Warrior(const std::string& elem, int str, const std::vector<std::string>& tr)
    : strength(str)
{
    element = elem;
    traits = tr;
}

void Warrior::info() const {
    std::cout << "Warrior | Element: " << element << ", Strength: " << strength << "\nTraits: ";
    for (const auto& t : traits) std::cout << t << " ";
    std::cout << std::endl;
}

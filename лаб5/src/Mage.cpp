#include "Mage.h"

Mage::Mage(const std::string& elem, int m, const std::vector<std::string>& tr)
    : mana(m)
{
    element = elem;
    traits = tr;
}

void Mage::info() const {
    std::cout << "Mage | Element: " << element << ", Mana: " << mana << "\nTraits: ";
    for (const auto& t : traits) std::cout << t << " ";
    std::cout << std::endl;
}

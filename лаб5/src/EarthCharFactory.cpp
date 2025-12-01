#include "EarthCharFactory.h"
#include "Archer.h"

std::unique_ptr<Character> EarthCharFactory::createWarrior() const {
    std::vector<std::string> traits = {"Сильный", "Выносливый", "Боится высоты"};
    return std::make_unique<Warrior>("Earth", 80, traits);
}

std::unique_ptr<Character> EarthCharFactory::createMage() const {
    std::vector<std::string> traits = {"Мудрый", "Сосредоточенный"};
    return std::make_unique<Mage>("Earth", 100, traits);
}

std::unique_ptr<Character> EarthCharFactory::createArcher() const {
    std::vector<std::string> traits = {"Меткий", "Быстрый"};
    return std::make_unique<Archer>("Earth", 85, traits);
}

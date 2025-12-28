#include "WaterCharFactory.h"
#include "Archer.h"

std::unique_ptr<Character> WaterCharFactory::createWarrior() const {
    std::vector<std::string> traits = {"Гибкий", "Умеет нырять"};
    return std::make_unique<Warrior>("Water", 70, traits);
}

std::unique_ptr<Character> WaterCharFactory::createMage() const {
    std::vector<std::string> traits = {"Ловкий", "Манипулирует водой"};
    return std::make_unique<Mage>("Water", 120, traits);
}

std::unique_ptr<Character> WaterCharFactory::createArcher() const {
    std::vector<std::string> traits = {"Стреляет под водой"};
    return std::make_unique<Archer>("Water", 90, traits);
}

#include "EarthCharFactory.h"
#include "WaterCharFactory.h"
#include <vector>
#include <memory>

int main() {
    std::vector<std::unique_ptr<CharacterFactory>> factories;
    factories.push_back(std::make_unique<EarthCharFactory>());
    factories.push_back(std::make_unique<WaterCharFactory>());

    for (const auto& factory : factories) {
        auto warrior = factory->createWarrior();
        auto mage = factory->createMage();
        auto archer = factory->createArcher(); // создаем лучника

        warrior->info();
        mage->info();
        archer->info();
        std::cout << "----------------------\n";
    }

    return 0;
}

#include "EarthCharFactory.h"
#include "WaterCharFactory.h"
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<CharacterFactory> factory;

    int factionChoice;
    std::cout << "Выберите стихию:\n";
    std::cout << "1 — Earth\n";
    std::cout << "2 — Water\n";
    std::cout << "Ввод: ";
    std::cin >> factionChoice;

    switch (factionChoice) {
        case 1:
            factory = std::make_unique<EarthCharFactory>();
            break;
        case 2:
            factory = std::make_unique<WaterCharFactory>();
            break;
        default:
            std::cout << "Ошибка: неизвестная стихия!\n";
            return 0;
    }

    int charChoice;
    std::cout << "\nВыберите тип персонажа:\n";
    std::cout << "1 — Warrior\n";
    std::cout << "2 — Mage\n";
    std::cout << "3 — Archer\n";
    std::cout << "Ввод: ";
    std::cin >> charChoice;

    std::unique_ptr<Character> character;

    switch (charChoice) {
        case 1:
            character = factory->createWarrior();
            break;
        case 2:
            character = factory->createMage();
            break;
        case 3:
            character = factory->createArcher();
            break;
        default:
            std::cout << "Ошибка: неизвестный тип персонажа!\n";
            return 0;
    }

    std::cout << "\nСоздан персонаж:\n";
    character->info();

    return 0;
}
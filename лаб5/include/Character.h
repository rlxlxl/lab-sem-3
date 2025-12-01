#pragma once
#include <string>
#include <iostream>
#include <vector>

class Character {
protected:
    std::string element;
    std::vector<std::string> traits; // уникальные характеристики
public:
    virtual ~Character() = default;
    virtual void info() const = 0;
    void addTrait(const std::string& trait) { traits.push_back(trait); }
};

#pragma once
#include "Istota.h"
#include <string>

class Potwor : public Istota
{
private:
    std::string description;
public:
    Potwor(std::string name, char symbol, int hp, int attack, int defense, std::string description);
    ~Potwor() = default;

    std::string getDescription() const { return description; }
    void printInfo() const;
};

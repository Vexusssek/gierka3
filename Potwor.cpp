#include "Potwor.h"
#include <cstdio>
#include <algorithm>

Potwor::Potwor(std::string name, char symbol, int hp, int attack, int defense, std::string description)
    : Istota(name, symbol, 0, 0, hp, attack, defense),
      description(description){}

void Potwor::printInfo() const 
{
    printf("Nazwa: %s\nSymbol: %c\nHP: %d\nAtak: %d\nObrona: %d\nOpis: %s\n", 
        name.c_str(), symbol, hp, attack, defense, description.c_str());
}
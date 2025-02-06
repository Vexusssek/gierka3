#include "Przedmiot.h"

Przedmiot::Przedmiot(std::string name, char symbol, int x, int y) 
    : name(name), symbol(symbol), x(x), y(y), cost(0), effectDescription("") {}

Przedmiot::Przedmiot(std::string name, char symbol, int x, int y, int cost, std::string effectDescription)
    : name(name), symbol(symbol), x(x), y(y), cost(cost), effectDescription(effectDescription) {}

#pragma once
#include <string>
#include <cstdio>

class Przedmiot
{
private:
    std::string name;
    char symbol;
    int x, y;
    int cost;
    std::string effectDescription;
    
public:
    Przedmiot(std::string name, char symbol, int x, int y);
    Przedmiot(std::string name, char symbol, int x, int y, int cost, std::string effectDescription);
    ~Przedmiot() = default;    
    
    std::string getName() const { return name; }
    char getSymbol() const { return symbol; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getItemCost() const { return cost; }
    std::string getItemDescription() const { return effectDescription; }
};

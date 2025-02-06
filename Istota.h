#pragma once
#include <string>

class Istota 
{
protected:
    std::string name;
    char symbol;
    int x, y;
    int hp;
    int maxHp;
    int attack;
    int defense;
    bool hasHealed;

public:
    Istota(std::string name, char symbol, int x, int y, int hp, int attack, int defense);
    virtual ~Istota() = default;

    void damage(int amount);
    bool isAlive() const;
    bool attackSys(Istota& target);
    void setPosition(int newX, int newY);

    std::string getName() const { return name; }
    char getSymbol() const { return symbol; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    void heal(int amount);
    bool hasAlreadyHealed() const { return hasHealed; }
    void setHealed(bool healed) { hasHealed = healed; }
    void applyRawDamage(int amount);
};
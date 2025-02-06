#include "Istota.h"
#include <algorithm>

Istota::Istota(std::string name, char symbol, int x, int y, int hp, int attack, int defense)
    : name(name), symbol(symbol), x(x), y(y), hp(hp), maxHp(hp), 
      attack(attack), defense(defense), hasHealed(false) {}
      
void Istota::setPosition(int newX, int newY) 
{
    x = newX;
    y = newY;
}

bool Istota::attackSys(Istota& target) 
{
    target.damage(attack);
    return !target.isAlive();
}

void Istota::damage(int amount) 
{
    hp = std::max(0, hp - std::max(0, amount - defense));
}

bool Istota::isAlive() const 
{
    return hp > 0;
}

void Istota::heal(int amount) 
{
    hp = std::min(hp + amount, maxHp);
}

void Istota::applyRawDamage(int amount) 
{
    hp = std::max(0, hp - amount);
}

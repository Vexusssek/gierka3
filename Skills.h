#pragma once
#include <string>
#include "Kostka.h"

class Skill 
{
private:
    std::string name;
    int manaCost;
    int bonusDamageMin;
    int bonusDamageMax;
    int manaRestoreMin;
    int manaRestoreMax;

public:
    Skill(std::string name, int manaCost, int bonusDmgMin, int bonusDmgMax, 
          int manaRestMin, int manaRestMax)
        : name(name), manaCost(manaCost), 
          bonusDamageMin(bonusDmgMin), bonusDamageMax(bonusDmgMax),
          manaRestoreMin(manaRestMin), manaRestoreMax(manaRestMax) {}

    std::string getName() const { return name; }
    int getManaCost() const { return manaCost; }
    
    int getBonusDamage() const 
    {
        Kostka dice(bonusDamageMax - bonusDamageMin + 1);
        return bonusDamageMin + dice.rzut() - 1;
    }
    
    int getManaRestore() const 
    {
        Kostka dice(manaRestoreMax - manaRestoreMin + 1);
        return manaRestoreMin + dice.rzut() - 1;
    }
};
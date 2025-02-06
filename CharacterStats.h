#pragma once
#include "Kostka.h"
#include <algorithm>

class CharacterStats 
{
private:
    int level;
    int baseDamage;
    int currentHp;
    int maxHp;
    int armor;
    int currentMana;
    int maxMana;
    int experience;
    int bonusDamage;
    int expToNextLevel;
    int gold;

public:
    CharacterStats(int damage = 5, int hp = 20, int def = 2, int mana = 10)
        : level(1), baseDamage(damage), currentHp(hp), maxHp(hp), armor(def),
          currentMana(mana), maxMana(mana), experience(0), bonusDamage(0), 
          expToNextLevel(20), gold(20) {}

    void getDisplayData(std::vector<std::string>& output) const;


    void damage(int amount) 
    { 
        currentHp = std::max(0, currentHp - std::max(0, amount - armor)); 
    }
    
    bool isDead() const { return currentHp <= 0; }
    
    void addExperience(int exp) 
    {
        experience += exp;
        checkLevelUp();
    }

    void checkLevelUp() 
    {
        if(experience >= expToNextLevel) 
        {
            levelUp();
        }
    }

    void levelUp() 
    {
        level++;
        experience -= expToNextLevel;
        expToNextLevel = 20 * level;

        Kostka k5(5);
        int hpIncrease = k5.rzut();
        int dmgIncrease = k5.rzut();
        int manaIncrease = 5 + k5.rzut();

        maxHp += hpIncrease;
        currentHp = maxHp;
        baseDamage += dmgIncrease;
        maxMana += manaIncrease;
        currentMana = maxMana;

        printf("\nAWANS NA POZIOM %d!\n", level);
        printf("HP +%d\n", hpIncrease);
        printf("Obrażenia +%d\n", dmgIncrease);
        printf("Mana +%d\n", manaIncrease);
    }

    void addBonusDamage(int bonus) { bonusDamage += bonus; }
    void heal(int amount) { currentHp = std::min(currentHp + amount, maxHp); }
    void increaseMana(int amount) { maxMana += amount; currentMana += amount; }
    void useMana(int amount) { currentMana = std::max(0, currentMana - amount); }
    void restoreMana(int amount) { currentMana = std::min(maxMana, currentMana + amount); }
    void addArmor(int amount) { armor += amount; }

    int getLevel() const { return level; }
    int getTotalDamage() const { return baseDamage + bonusDamage; }
    int getCurrentHp() const { return currentHp; }
    int getMaxHp() const { return maxHp; }
    int getArmor() const { return armor; }
    int getCurrentMana() const { return currentMana; }
    int getMaxMana() const { return maxMana; }
    void addGold(int amount) { gold += amount; }
    bool removeGold(int amount) 
    {
        if(gold >= amount) 
        {
            gold -= amount;
            return true;
        }
        return false;
    }
    int getGold() const { return gold; }

    void display() const {
        printf("\nStatystyki postaci:\n");
        printf("Poziom: %d\n", level);
        printf("HP: %d/%d\n", currentHp, maxHp);
        printf("Mana: %d/%d\n", currentMana, maxMana);
        printf("Obrażenia: %d (Bazowe: %d + Bonus: %d)\n", getTotalDamage(), baseDamage, bonusDamage);
        printf("Pancerz: %d\n", armor);
        printf("Doświadczenie: %d/%d\n", experience, expToNextLevel);
        printf("Złoto: %d\n", gold);
    }
    
};

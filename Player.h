#pragma once
#include "Istota.h"
#include "Ekwipunek.h"
#include "CharacterStats.h"
#include "Platform.h"
#include "Skills.h"
#include <vector>

class Player : public Istota 
{
private:
    Ekwipunek ekwipunek;
    CharacterStats stats;
    std::vector<Skill> skills;

public:
    Player(std::string name, char symbol, int x, int y, int hp, int attack, int defense);
    
    void damage(int amount) 
    {
        Istota::damage(amount);
        stats.damage(amount);
        hp = stats.getCurrentHp();
    }
    
    bool isAlive() const 
    {
        return stats.getCurrentHp() > 0;
    }

    void move(int newX, int newY);
    bool pickupItem(const Przedmiot& item);
    void applyItemEffect(const Przedmiot& item);
    void addSkill(const Skill& skill);
    bool hasSkills() const { return !skills.empty(); }
    void displaySkills() const;
    bool useSkill(int index, Istota& target);
    const Ekwipunek& getEkwipunek() const { return ekwipunek; }
    const CharacterStats& getStats() const { return stats; }
    CharacterStats& getStats() { return stats; }
    size_t getSkillsCount() const { return skills.size(); }
};
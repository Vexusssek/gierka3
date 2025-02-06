#include "Player.h"

Player::Player(std::string name, char symbol, int x, int y, int hp, int attack, int defense)
    : Istota(name, symbol, x, y, hp, attack, defense), 
      stats(attack, hp, defense, 10) {}

void Player::move(int newX, int newY) 
{
    setPosition(newX, newY);
}

void Player::displaySkills() const 
{
    printf("\nUmiejętności:\n");
    for(size_t i = 0; i < skills.size(); i++) 
    {
        printf("%zu. %s (Koszt many: %d)\n", 
               i + 1, 
               skills[i].getName().c_str(), 
               skills[i].getManaCost());
    }
}

bool Player::useSkill(int index, Istota& target) 
{
    if(index >= 0 && index < skills.size()) 
    {
        const Skill& skill = skills[index];
        if(stats.getCurrentMana() >= skill.getManaCost()) 
        {
            int totalDamage = stats.getTotalDamage() + skill.getBonusDamage();
            target.damage(totalDamage);
            stats.useMana(skill.getManaCost());
            stats.restoreMana(skill.getManaRestore());
            printf("%s używa %s i zadaje %d obrażeń!\n", 
                   getName().c_str(), skill.getName().c_str(), totalDamage);
            return true;
        }
        printf("Za mało many!\n");
    }
    return false;
}

void Player::applyItemEffect(const Przedmiot& item) 
{
    if(item.getName() == "Health Potion") 
    {
        if(getStats().getCurrentMana() >= 5) 
        {
            if(getStats().getCurrentHp() < getStats().getMaxHp()) 
            {
                getStats().heal(5);
                getStats().useMana(5);
                printf("\nUleczono 5 HP!\n");
            } 
            else 
            {
                printf("\nMasz już maksymalne HP!\n");
            }
        } 
        else 
        {
            printf("\nZa mało many!\n");
        }
    }
    else if(item.getName() == "Magic Wand") 
    {
        getStats().addBonusDamage(5);
        printf("\nZdobyłeś Magic Wand! +5 do obrażeń!\n");
    }
    else if(item.getName() == "Spellbook") 
    {
        getStats().increaseMana(5);
        getStats().addBonusDamage(5);
        addSkill(Skill("Magic Blast", 7, 1, 3, 2, 4));
        printf("\nZdobyłeś Spellbook! +5 do many, +5 do obrażeń i umiejętność Magic Blast!\n");
    }
    else if(item.getName() == "Sword of Flames")
    {
        int bonus = getStats().getTotalDamage() / 2;
        getStats().addBonusDamage(bonus);
        printf("\nZdobyłeś Sword of Flames! Obrażenia wzrosły o %d!\n", bonus);
    }
    else if(item.getName() == "Shield of Wisdom")
    {
        getStats().addArmor(2);
        printf("\nZdobyłeś Shield of Wisdom! Pancerz wzrósł o 2!\n");
    }
    else if(item.getName() == "Mana Elixir")
    {
        getStats().restoreMana(10);
        printf("\nZdobyłeś Mana Elixir! Odzyskałeś 10 many!\n");
    }
    else if(item.getName() == "Ring of Power")
    {
        getStats().addBonusDamage(3);
        getStats().increaseMana(3);
        printf("\nZdobyłeś Ring of Power! Obrażenia +3 i mana +3!\n");
    }
}

bool Player::pickupItem(const Przedmiot& item) 
{
    if(ekwipunek.addItem(item)) 
    {
        applyItemEffect(item);
        return true;
    }
    return false;
}

void Player::addSkill(const Skill& skill) 
{
    skills.push_back(skill);
}
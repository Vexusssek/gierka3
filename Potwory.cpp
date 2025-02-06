#include "Potwory.h"
#include <cstdio>

Potwory::Potwory() 
{
    monsters.emplace_back("Ice Minion", 'I', 8, 1, 0, 
        "Słaby przeciwnik, stanowi zagrożenie tylko w grupie.");
    monsters.emplace_back("Banished One", 'O', 10, 3, 1, 
        "Samotnie raczej przegrywa, ale wyrządza krzywdę.");
    monsters.emplace_back("Flamethrower", 'F', 20, 7, 3, 
        "Równe szanse");
    monsters.emplace_back("Believer", 'B', 50, 8, 4, "Ogromne zagrożenie, do pokonania, czasem, przy dużym szczęściu.");
    monsters.emplace_back("Ancient Magician", 'Y', 90, 10, 8, "Brak szans na początku rozgrywki.");
}

void Potwory::printAllMonsters() const 
{
    for (const auto& monster : monsters) 
    {
        monster.printInfo();
        printf("\n");
    }
}


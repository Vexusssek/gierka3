#include "Walka.h"
#include "Platform.h"
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

bool Walka::rozpocznijWalke(Player& player, Istota& przeciwnik) 
{
    while(player.isAlive() && przeciwnik.isAlive()) 
    {
        clearScreen();
        printf("\nTwoja tura!\n");
        printf("Twoje HP: %d/%d  Mana: %d/%d\n", 
               player.getStats().getCurrentHp(), player.getStats().getMaxHp(),
               player.getStats().getCurrentMana(), player.getStats().getMaxMana());
        printf("HP przeciwnika (%s): %d/%d\n", 
               przeciwnik.getName().c_str(), przeciwnik.getHp(), przeciwnik.getMaxHp());

        if(player.getStats().isDead()) 
        {
            printf("\nGAME OVER!\n");
            #ifdef _WIN32
            system("pause");
            #else
            std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
            getchWrapper();
            #endif
            return false;
        }

        printf("\nWybierz akcję:\n");
        printf("1. Zwykły atak\n");
        if(player.hasSkills()) 
        {
            printf("2. Użyj umiejętności\n");
        }
        
        char choice = getchWrapper();
        if(choice == '1') 
        {
            printf("\nAtakujesz %s!\n", przeciwnik.getName().c_str());
            int playerDamage = std::max(1, player.getAttack() - przeciwnik.getDefense());
            przeciwnik.applyRawDamage(playerDamage);
            printf("Zadajesz %d obrażeń!\n", playerDamage);

            if(!przeciwnik.isAlive()) 
            {
                Kostka d10(10);
                Kostka d5(5);
                int goldReward = d10.rzut() + d5.rzut();
                player.getStats().addGold(goldReward);

                Kostka expDice(10);
                int expGain = 5 + expDice.rzut();
                player.getStats().addExperience(expGain);

                printf("\nZwyciężyłeś!\n");
                printf("Znaleziono %d złota!\n", goldReward);
                printf("Zdobyto %d doświadczenia!\n", expGain);
                #ifdef _WIN32
                system("pause");
                #else
                std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
                getchWrapper();
                #endif
                return true;
            }
        }
        else if(choice == '2' && player.hasSkills()) 
        {
            player.displaySkills();
            printf("Wybierz umiejętność (1-%zu): ", player.getSkillsCount());
            char skillChoice = getchWrapper();
            int skillIndex = skillChoice - '1';
            
            if(player.useSkill(skillIndex, przeciwnik)) 
            {
                if(!przeciwnik.isAlive()) 
                {
                    Kostka d10(10);
                    Kostka d5(5);
                    int goldReward = d10.rzut() + d5.rzut();
                    player.getStats().addGold(goldReward);
                    printf("\nZnaleziono %d złota!\n", goldReward);

                    Kostka expDice(10);
                    int expGain = 5 + expDice.rzut();
                    player.getStats().addExperience(expGain);
                    printf("\nZdobyto %d doświadczenia!\n", expGain);
                    #ifdef _WIN32
                    system("pause");
                    #else
                    std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
                    getchWrapper();
                    #endif
                    return true;
                }
            }
        }
        
        if(przeciwnik.isAlive()) 
        {
            printf("\nTura przeciwnika!\n");
            int enemyDamage = std::max(1, przeciwnik.getAttack() - player.getDefense());
            player.damage(enemyDamage);
            printf("%s zadał Ci %d obrażeń!\n", przeciwnik.getName().c_str(), enemyDamage);

            if(player.getStats().isDead()) 
            {
                printf("\nGAME OVER!\n");
                #ifdef _WIN32
                system("pause");
                #else
                std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
                getchWrapper();
                #endif
                return false;
            }
            
            // Banished One – możliwość ucieczki
            if (przeciwnik.getName() == "Banished One" && przeciwnik.getHp() < (przeciwnik.getMaxHp() * 0.7))
            {
                Kostka escapeChance(10);
                if (escapeChance.rzut() <= 4) 
                {
                    printf("\nO nie! Banished One uciekł...\n");
                    #ifdef _WIN32
                    system("pause");
                    #else
                    std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
                    getchWrapper();
                    #endif
                    return true;
                }
            }

            // Flamethrower – leczenie
            if (przeciwnik.getName() == "Flamethrower" && przeciwnik.getHp() < 10) 
            {
                if (!przeciwnik.hasAlreadyHealed())
                {
                    Kostka healChance(10);
                    if (healChance.rzut() <= 4)
                    {
                        int maxHp = przeciwnik.getMaxHp();
                        int healAmount = maxHp * 0.5;
                        przeciwnik.heal(healAmount);
                        przeciwnik.setHealed(true);
                        printf("%s odnowił sobie %d HP!\n", przeciwnik.getName().c_str(), healAmount);
                    }
                }
            }

            // Ice Minion – krytyczny atak
            if (przeciwnik.getName() == "Ice Minion" && przeciwnik.getHp() < 8) 
            {
                Kostka healChance(10);
                if (healChance.rzut() <= 6)
                {
                    int dmg = std::max(1, (przeciwnik.getAttack()) * 5);
                    player.damage(dmg);
                    printf("%s Atak krytyczny! Zadał Ci %d obrażeń!\n", przeciwnik.getName().c_str(), dmg);
                }
            }

            #ifdef _WIN32
            system("pause");
            #else
            std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
            getchWrapper();
            #endif
        }
    }
    return player.isAlive();
}

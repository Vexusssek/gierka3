#include "Shop.h"
#include "Przedmiot.h"
#include "Map.h"
#include "Player.h"
#include "Platform.h"
#include <cstdlib>
#include <cstdio>

Shop::Shop() 
{
    positions.push_back({-1, 0});
    positions.push_back({1, 0});
    positions.push_back({0, 1});
}

void Shop::generateShopItems(Map* map, int merchantX, int merchantY) 
{
    shopItems.push_back(Przedmiot("Health Potion", 'p', merchantX, merchantY, 10, "Leczy 5 HP (koszt: 5 many)"));
    shopItems.push_back(Przedmiot("Magic Wand", 'w', merchantX, merchantY, 15, "Daje +5 do obrażeń"));
    shopItems.push_back(Przedmiot("Spellbook", 's', merchantX, merchantY, 20, "Daje +5 do many, +5 do obrażeń i umiejętność Magic Blast"));
    shopItems.push_back(Przedmiot("Sword of Flames", 'S', merchantX, merchantY, 25, "Zwiększa obrażenia o 50%"));
    shopItems.push_back(Przedmiot("Shield of Wisdom", 'D', merchantX, merchantY, 20, "Zwiększa pancerz o 2"));
    shopItems.push_back(Przedmiot("Mana Elixir", 'm', merchantX, merchantY, 15, "Przywraca 10 many"));
    shopItems.push_back(Przedmiot("Ring of Power", 'R', merchantX, merchantY, 30, "Daje +3 do obrażeń i +3 do many"));

    std::vector<Przedmiot> chosenItems;
    std::vector<std::string> availableNames;
    for (const auto& item : shopItems)
        availableNames.push_back(item.getName());

    chosenItems.clear();
    int itemsToSelect = (positions.size() < 3 ? positions.size() : 3);
    for (int i = 0; i < itemsToSelect && !availableNames.empty(); i++) 
    {
        int randomIndex = rand() % availableNames.size();
        std::string selectedName = availableNames[randomIndex];
        for (const auto& item : shopItems) 
        {
            if (item.getName() == selectedName) 
            {
                int posX = merchantX + positions[i].first;
                int posY = merchantY + positions[i].second;
                Przedmiot chosenItem(item.getName(), item.getSymbol(), posX, posY, item.getItemCost(), item.getItemDescription());
                chosenItems.push_back(chosenItem);
                break;
            }
        }
        availableNames.erase(availableNames.begin() + randomIndex);
    }

    for (const auto& item : chosenItems) 
    {
        if (map->placable(item.getX(), item.getY())) 
        {
            map->placeItem(item, item.getX(), item.getY());
        }
    }
}

bool Shop::attemptPurchase(Player* player, Map* map) 
{
    const std::vector<Przedmiot>& itemsInShop = map->getItems();
    for (const auto& item : itemsInShop) 
    {
        if ((abs(item.getX() - player->getX()) <= 1) && (abs(item.getY() - player->getY()) <= 1)) 
        {
            printf("\nPrzedmiot: %s\n", item.getName().c_str());
            printf("Efekt: %s\n", item.getItemDescription().c_str());
            printf("Cena: %d złota\n", item.getItemCost());
            printf("Czy chcesz kupić? (t/n)\n");
            char choice = getchWrapper();
            if (choice == 't' || choice == 'T') 
            {
                if (player->getStats().getGold() >= item.getItemCost()) 
                {
                    if (player->getStats().removeGold(item.getItemCost())) 
                    {
                        if (player->pickupItem(item)) 
                        {
                            printf("\nKupiono %s!\n", item.getName().c_str());
                            map->removeItem(item.getX(), item.getY());
                        } else {
                            printf("\nNie możesz kupić %s, ekwipunek pełny lub już posiadasz ten przedmiot.\n", item.getName().c_str());
                        }
                    }
                } 
                else 
                {
                    printf("\nZa mało złota!\n");
                }
                return true;
            }
        }
    }
    return false;
}
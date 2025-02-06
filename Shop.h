#pragma once
#include <vector>
#include <string>
#include "Przedmiot.h"

class Map;
class Player;

class Shop 
{
private:
    std::vector<Przedmiot> shopItems;
    std::vector<std::pair<int, int>> positions;
public:
    Shop();
    void generateShopItems(Map* map, int merchantX, int merchantY);
    bool attemptPurchase(Player* player, Map* map);
};
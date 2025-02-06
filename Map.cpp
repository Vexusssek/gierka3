#include "Map.h"
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <random>

Map::Map(int width, int height, RoomType type, int bossIndex, int floorLevel)
    : width(width), height(height), currentType(type), floorLevel(floorLevel)
{
    if (width == 0 || height == 0) 
    {
        switch(type) 
        {
            case START_ROOM:
                this->width = this->height = START_ROOM_SIZE;
                break;
            case SHOP_ROOM:
                this->width = this->height = SHOP_ROOM_SIZE;
                break;
            case BOSS_ROOM:
                this->width = this->height = BOSS_ROOM_SIZE;
                break;
            case STANDARD_ROOM:
                if(floorLevel > 1)
                {
                    int extra = (floorLevel - 1) * 2;
                    this->width = rand() % (MAX_STANDARD_SIZE - MIN_STANDARD_SIZE + 1) + MIN_STANDARD_SIZE + extra;
                    this->height = rand() % (MAX_STANDARD_SIZE - MIN_STANDARD_SIZE + 1) + MIN_STANDARD_SIZE + extra;
                }
                else
                {
                    this->width = rand() % (MAX_STANDARD_SIZE - MIN_STANDARD_SIZE + 1) + MIN_STANDARD_SIZE;
                    this->height = rand() % (MAX_STANDARD_SIZE - MIN_STANDARD_SIZE + 1) + MIN_STANDARD_SIZE;
                }
                break;
        }
    }

    map = new char*[this->height];
    for(int i = 0; i < this->height; ++i) 
    {
        map[i] = new char[this->width];
        for(int j = 0; j < this->width; ++j) 
        {
            if(i == 0 || i == this->height-1 || j == 0 || j == this->width-1)
                map[i][j] = WALL;
            else
                map[i][j] = EMPTY;
        }
    }

    switch(type) 
    {
        case START_ROOM:
            generateStartRoom();
            placeDoors(1);
            break;
        case STANDARD_ROOM:
            generateStandardRoom();
            placeDoors(2);
            break;
        case SHOP_ROOM:
            generateShopRoom();
            placeDoors(2);
            break;
        case BOSS_ROOM:
            generateBossRoom(bossIndex);
            placeDoors(1);
            break;
    }
}

Map::~Map() 
{
    for(int i = 0; i < height; ++i) 
    {
        delete[] map[i];
    }
    delete[] map;
}

void Map::print() 
{
    for(int i = 0; i < height; ++i) 
    {
        for(int j = 0; j < width; ++j) 
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

bool Map::placable(int x, int y) 
{
    if(x >= 0 && x < width && y >= 0 && y < height) 
    {
        return map[y][x] == EMPTY;
    }
    return false;
}

bool Map::isDoor(int x, int y) const 
{
    if(x >= 0 && x < width && y >= 0 && y < height) 
    {
        return map[y][x] == DOOR;
    }
    return false;
}

void Map::placeItem(const Przedmiot& item, int x, int y) 
{
    if(placable(x, y)) {
        map[y][x] = item.getSymbol();
        items.push_back(item);
    }
}

void Map::placeEntity(const Istota& entity, int x, int y) 
{
    if(placable(x, y)) 
    {
        map[y][x] = entity.getSymbol();
        istoty.push_back(entity);
    }
}

void Map::removeEntity(int x, int y) 
{
    if(x >= 0 && x < width && y >= 0 && y < height) 
    {
        if(map[y][x] != WALL && map[y][x] != DOOR) 
        {
            map[y][x] = EMPTY;
            istoty.erase(
                std::remove_if(istoty.begin(), istoty.end(),
                    [x, y](const Istota& entity) {
                        return entity.getX() == x && entity.getY() == y;
                    }
                ),
                istoty.end()
            );
        }
    }
}

void Map::placeMonster(const Potwor& monster, int x, int y) 
{
    if(placable(x, y)) 
    {
        map[y][x] = monster.getSymbol();
        istoty.push_back(Istota(monster.getName(), monster.getSymbol(), x, y,
                               monster.getHp(), monster.getAttack(), monster.getDefense()));
    }
}


//Walka do testów
int Map::battle(char symbol1, char symbol2) 
{
    Istota* entity1 = nullptr;
    Istota* entity2 = nullptr;

    for(auto& entity : istoty) 
    {
        if(entity.getSymbol() == symbol1)
            entity1 = &entity;
        else if(entity.getSymbol() == symbol2)
            entity2 = &entity;
    }

    if(!entity1 || !entity2)
        return 0;

    int round = 1;
    while(entity1->isAlive() && entity2->isAlive()) 
    {
        std::cout << "\nRunda " << round << ":\n";
        std::cout << entity1->getName() << " (HP: " << entity1->getHp() << ") vs " 
                  << entity2->getName() << " (HP: " << entity2->getHp() << ")\n";

        std::cout << entity1->getName() << " atakuje " << entity2->getName() << "...\n";
        bool killed = entity1->attackSys(*entity2);
        std::cout << entity2->getName() << " ma teraz " << entity2->getHp() << " HP.\n";
        if(killed)
        {
            std::cout << entity2->getName() << " zginął!\n";
            return 1;
        }
        
        std::cout << entity2->getName() << " atakuje " << entity1->getName() << "...\n";
        killed = entity2->attackSys(*entity1);
        std::cout << entity1->getName() << " ma teraz " << entity1->getHp() << " HP.\n";
        if(killed)
        {
            std::cout << entity1->getName() << " zginął!\n";
            return -1;
        }

        round++;
        std::cout << "Naciśnij ENTER, aby kontynuować kolejną rundę...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    return 0;
}

void Map::generateStartRoom() 
{
    int centerX = width / 2;
    int centerY = height / 2;
    map[height-1][centerX] = DOOR;
    doors.push_back({centerX, height-1});
}

void Map::generateStandardRoom() 
{
    int centerX = width / 2;
    map[0][centerX] = DOOR;
    map[height-1][centerX] = DOOR;
    doors.push_back({centerX, 0});
    doors.push_back({centerX, height-1});
    
    int numMonsters = rand() % 3;
    if(floorLevel > 1)
    {
        numMonsters += floorLevel;
    }

    Potwory monsters;
    for(int i = 0; i < numMonsters; i++) 
    {
        int x = rand() % (width-2) + 1;
        int y = rand() % (height-2) + 1;
        if(placable(x, y)) 
        {
            const Potwor& monster = monsters.getMonsters()[rand() % 2];
            placeMonster(monster, x, y);
        }
    }

    int numRivers = 1;
    for(int i = 0; i < numRivers; i++) 
    {
        int x, y;
        bool validPosition = false;
        for(int attempts = 0; attempts < 100 && !validPosition; attempts++) 
        {
            x = rand() % (width - 2) + 1;
            y = rand() % (height - 2) + 1;
            bool nearDoor = false;
            for(const Door& door : doors) 
            {
                if(abs(x - door.x) <= 1 && abs(y - door.y) <= 1) 
                {
                    nearDoor = true;
                    break;
                }
            }
            if(!nearDoor && placable(x, y)) 
            {
                validPosition = true;
                map[y][x] = RIVER;
            }
        }
    }
}

void Map::generateShopRoom() 
{
    int centerX = width / 2;
    int centerY = height / 2;
    
    map[0][centerX] = DOOR;
    map[height-1][centerX] = DOOR;
    doors.push_back({centerX, 0});
    doors.push_back({centerX, height-1});
    
    Istota merchant("Merchant", 'M', centerX, centerY, 100, 0, 10);
    placeEntity(merchant, centerX, centerY);
    
    shop.generateShopItems(this, centerX, centerY);
}

void Map::removeItem(int x, int y) 
{
    if(x >= 0 && x < width && y >= 0 && y < height) 
    {
        map[y][x] = EMPTY;
        items.erase(
            std::remove_if(items.begin(), items.end(),
                [x, y](const Przedmiot& item) 
                {
                    return item.getX() == x && item.getY() == y;
                }
            ),
            items.end()
        );
    }
}

void Map::generateBossRoom(int bossIndex)
{
    int centerX = width / 2;
    map[0][centerX] = DOOR;
    doors.push_back({centerX, 0});

    Potwory monsters;
    const Potwor& boss = monsters.getMonsters()[bossIndex];
    placeMonster(boss, width/2, height/2);
}

void Map::placeDoors(int numDoors) 
{
    //??? chyba nie
}

bool Map::isValidDoorPosition(int x, int y) 
{
    if(x != 0 && x != width-1 && y != 0 && y != height-1) 
        return false;
    
    for(const Door& door : doors) 
    {
        if(abs(door.x - x) + abs(door.y - y) < 2)
            return false;
    }
    return true;
}

void Map::raport() 
{
    printf("Items in room:\n");
    for(const auto& item : items) 
    {
        printf("%s at (%d,%d)\n", 
               item.getName().c_str(), 
               item.getX(), 
               item.getY());
    }
    printf("\nEntities in room:\n");
    for(const auto& entity : istoty) 
    {
        printf("%s at (%d,%d)\n", 
               entity.getName().c_str(), 
               entity.getX(), 
               entity.getY());
    }
}

bool Map::isRiver(int x, int y) const 
{
    if(x >= 0 && x < width && y >= 0 && y < height) 
    {
        return map[y][x] == RIVER;
    }
    return false;
}

void Map::placeTile(int x, int y, floor_symbols tile) 
    { 
        if(x >= 0 && x < width && y >= 0 && y < height) 
        {
            map[y][x] = tile;
        }
    }
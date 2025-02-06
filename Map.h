#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Istota.h"
#include "Przedmiot.h"
#include "Potwory.h"
#include "Shop.h"

enum RoomType 
{
    START_ROOM,
    STANDARD_ROOM,
    SHOP_ROOM,
    BOSS_ROOM
};

enum floor_symbols 
{
    EMPTY = '.',
    WALL = '#',
    DOOR = '+',
    RIVER = '~',
    TRAPDOOR = 'X'
};

struct Door 
{
    int x, y;
    char symbol = '+';
};

class Map 
{
private:
    char** map;
    int width;
    int height;
    std::vector<Przedmiot> items;
    void getDisplayData(std::vector<std::string>& output) const;
    std::vector<Istota> istoty;
    std::vector<Door> doors;
    RoomType currentType;
    int floorLevel;
    Shop shop;

    static const int START_ROOM_SIZE = 8;
    static const int SHOP_ROOM_SIZE = 9;
    static const int MIN_STANDARD_SIZE = 6;
    static const int MAX_STANDARD_SIZE = 10;
    static const int BOSS_ROOM_SIZE = 12;

public:
    Map(int width, int height, RoomType type = START_ROOM, int bossIndex = 2, int floorLevel = 1);
    ~Map();
    void print();
    bool placable(int x, int y);
    bool isDoor(int x, int y) const;
    void placeItem(const Przedmiot& item, int x, int y);
    void placeEntity(const Istota& entity, int x, int y);
    void removeItem(int x, int y);
    void removeEntity(int x, int y);
    void raport();
    int battle(char symbol1, char symbol2);
    void placeMonster(const Potwor& monster, int x, int y);
    const std::vector<Przedmiot>& getItems() const { return items;}
    const std::vector<Istota>& getEntities() const { return istoty; }
    std::vector<Istota>& getEntities() { return istoty; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    RoomType getType() const { return currentType; }
    bool isRiver(int x, int y) const;
    void placeTile(int x, int y, floor_symbols tile);
    char getTile(int x, int y) const { return map[y][x]; }
    

private:
    void generateStartRoom();
    void generateStandardRoom();
    void generateShopRoom();
    void generateBossRoom(int bossIndex);
    void placeDoors(int numDoors);
    bool isValidDoorPosition(int x, int y);
};
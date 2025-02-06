#pragma once
#include "Map.h"
#include "Player.h"
#include "Platform.h"
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <stdio.h>
#endif

class Game 
{
private:
    std::vector<Map*> rooms;
    Map* currentRoom;
    Player* player;
    bool isRunning;
    size_t currentRoomIndex;
    bool needsRedraw;
    int currentFloor;
    void generateNextFloor();


public:
    Game();
    ~Game();
    void init();
    void handleInput();
    void update();
    void render();
    bool running() const { return isRunning; }
    void showEnding();

private:
    void generateLevel();
    void clearLevel();
    void tryChangeRoom(int x, int y);
};
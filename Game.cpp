#include "Game.h"
#include "Walka.h"
#include "Platform.h"
#include <thread>
#include <chrono>
#include <iostream>

Game::Game() : currentRoom(nullptr), player(nullptr), 
               isRunning(false), currentRoomIndex(0), 
               needsRedraw(true), currentFloor(1)
{
    srand(time(nullptr));
}

Game::~Game() 
{
    clearLevel();
    delete player;
}

void Game::clearLevel() 
{
    for(auto room : rooms) 
    {
        delete room;
    }
    rooms.clear();
    currentRoomIndex = 0;
}

void Game::generateLevel() 
{
    clearLevel();
    rooms.push_back(new Map(0, 0, START_ROOM));
    
    int standardRooms = 1 + (rand() % 4);
    for(int i = 0; i < standardRooms; i++) 
    {
        rooms.push_back(new Map(0, 0, STANDARD_ROOM));
    }
    
    rooms.push_back(new Map(0, 0, SHOP_ROOM));
    rooms.push_back(new Map(0, 0, BOSS_ROOM));
    
    currentRoom = rooms[0];
}

void Game::init() 
{
    generateLevel();
    player = new Player("Player", '@', currentRoom->getWidth()/2, 
                       currentRoom->getHeight()/2,20,5,1);
    currentRoom->placeEntity(*player, player->getX(), player->getY());
    isRunning = true;
}

void Game::tryChangeRoom(int x, int y) 
{
    if(currentRoom->isDoor(x, y)) 
    {
        currentRoom->removeEntity(player->getX(), player->getY());
        
        if(y == currentRoom->getHeight() - 1 && currentRoomIndex < rooms.size() - 1) 
        {
            currentRoomIndex++;
            currentRoom = rooms[currentRoomIndex];
            int newX = currentRoom->getWidth() / 2;
            player->move(newX, 1);
            currentRoom->placeEntity(*player, newX, 1);
        }
        else if(y == 0 && currentRoomIndex > 0) 
        {
            currentRoomIndex--;
            currentRoom = rooms[currentRoomIndex];
            int newX = currentRoom->getWidth() / 2;
            player->move(newX, currentRoom->getHeight() - 2);
            currentRoom->placeEntity(*player, newX, currentRoom->getHeight() - 2);
        }
        needsRedraw = true;
    }
}

void Game::handleInput() 
{
    if (kbhitWrapper()) 
    {
        char input = getchWrapper();
        int newX = player->getX();
        int newY = player->getY();
        bool moved = false;

        if(input == 'e') 
        {
            bool itemFound = false;
            for(int dx = -1; dx <= 1; dx++) 
            {
                for(int dy = -1; dy <= 1; dy++) 
                {
                    int checkX = player->getX() + dx;
                    int checkY = player->getY() + dy;
                    if(currentRoom->isRiver(checkX, checkY)) {
                        printf("\nCzy chcesz wrzucić 5 złota? (t/n)\n");
                        char choice = getchWrapper();
                        if(choice == 't' && player->getStats().removeGold(5)) 
                        {
                            Kostka chance(10);
                            if(chance.rzut() == 1) 
                            {
                                player->getStats().addGold(10);
                                printf("\nZnalazłeś 10 złota w rzece!\n");
                            } else {
                                printf("\nNic nie znalazłeś...\n");
                            }
                        }
                        return;
                    }
                }
            }

            for(int dx = -1; dx <= 1; dx++) 
            {
                for(int dy = -1; dy <= 1; dy++) 
                {
                    int checkX = player->getX() + dx;
                    int checkY = player->getY() + dy;
                    
                    if(currentRoom->getTile(checkX, checkY) == TRAPDOOR) 
                    {
                        printf("\nCzy chcesz przejść na następne piętro? (t/n)\n");
                        char choice = getchWrapper();
                        if(choice == 't') 
                        {
                            generateNextFloor();
                            needsRedraw = true;
                            return;
                        }
                    }
                }
            }

    bool purchaseAttempt = false;
    for(const auto& item : currentRoom->getItems()) 
    {
        if((abs(item.getX() - player->getX()) <= 1) && (abs(item.getY() - player->getY()) <= 1))
        {
            std::string itemName = item.getName();
            int cost = item.getItemCost();
            std::string description = item.getItemDescription();
            
            printf("\nPrzedmiot: %s\n", itemName.c_str());
            printf("Efekt: %s\n", description.c_str());
            printf("Cena: %d złota\n", cost);
            printf("Czy chcesz kupić? (t/n)\n");
            char choice = getchWrapper();
            std::this_thread::sleep_for(std::chrono::milliseconds(150));

            if(choice == 't' || choice == 'T')
            {
                if(player->getStats().getGold() >= cost)
                {
                    if(player->getStats().removeGold(cost))
                    {
                        if(player->pickupItem(item))
                        {
                            printf("\nKupiono %s!\n", itemName.c_str());
                            currentRoom->removeItem(item.getX(), item.getY());
                        }
                        else 
                        {
                            printf("\nNie możesz kupić %s, ekwipunek pełny lub już posiadasz ten przedmiot.\n", itemName.c_str());
                        }
                    }
                }
                else 
                {
                    printf("\nZa mało złota!\n");
                }
            }
            while(kbhitWrapper()) { getchWrapper(); }
            purchaseAttempt = true;
            return;
        }
    }
    if(!purchaseAttempt)
    {
        printf("\nBrak przedmiotów w zasięgu.\n");
    }
    return;           
                    
        }

        else 
        {
            switch(input) 
            {
                case 'w': newY--; moved = true; break;
                case 's': newY++; moved = true; break;
                case 'a': newX--; moved = true; break;
                case 'd': newX++; moved = true; break;
                case 'q': isRunning = false; break;
                case 'r':
                for(auto& entity : currentRoom->getEntities()) 
                {
                if(entity.getName() == "Merchant")
                    continue;
                if(abs(entity.getX() - player->getX()) <= 1 && 
                 abs(entity.getY() - player->getY()) <= 1 &&
                  entity.getSymbol() != player->getSymbol()) 
                  {
                    if(Walka::rozpocznijWalke(*player, entity)) 
                    {
                        int entX = entity.getX();
                        int entY = entity.getY();
                        std::string nameE = entity.getName();

                        currentRoom->removeEntity(entX, entY);
                        printf("\nWygrałeś walkę!\n");

                        if(nameE == "Ancient Magician")
                        {
                        showEnding();
                        isRunning = false;
                        return;
                        }
                        
                        if(nameE == "Flamethrower" || nameE == "Believer") 
                        {
                            currentRoom->placeTile(entX, entY, TRAPDOOR);
                            needsRedraw = true;
                            printf("\nPojawił się właz do następnego piętra!\n");
                        }
        
                    } 
                    else 
                    {
                        printf("\nPrzegrałeś walkę!\n");
                        isRunning = false;
                    }
                        needsRedraw = true;
                        break;
                    }
                }
                    break;
            }
            if(moved) 
            {
                if(currentRoom->isDoor(newX, newY)) 
                {
                    tryChangeRoom(newX, newY);
                }
                else if(currentRoom->placable(newX, newY)) 
                {
                    currentRoom->removeEntity(player->getX(), player->getY());
                    player->move(newX, newY);
                    currentRoom->placeEntity(*player, newX, newY);
                    needsRedraw = true;
                }
            }
        }
    }
}

void Game::update() 
{
    if (!needsRedraw) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::render() 
{
    if (!needsRedraw) return;
    
    clearScreen();
    printf("\n\n");
    currentRoom->print();
    printf("\nSterowanie: WASD - ruch, E - podnieś przedmiot/aktywność, Q - wyjscie R - walcz\n");
    printf("Pozycja gracza: %d,%d\n", player->getX(), player->getY());
    printf("Pokoj %zu/%zu\n", currentRoomIndex + 1, rooms.size());
    printf("Piętro: %d\n", currentFloor);
    player->getEkwipunek().display();
    player->getStats().display();
    
    needsRedraw = false;
}

void Game::generateNextFloor() 
{
    currentFloor++;
    clearLevel();

    rooms.push_back(new Map(0, 0, START_ROOM, 2, currentFloor));
    
    int standardRooms = 1 + (rand() % 4);
    for(int i = 0; i < standardRooms; i++) 
    {
        rooms.push_back(new Map(0, 0, STANDARD_ROOM, 2, currentFloor));
    }
    
    rooms.push_back(new Map(0, 0, SHOP_ROOM, 2, currentFloor));
    
    int bossIndex;
    if(currentFloor == 2)
        bossIndex = 3;
    else if(currentFloor == 3)
        bossIndex = 4;
    else
        bossIndex = 2;
    Map* bossRoom = new Map(0, 0, BOSS_ROOM, bossIndex, currentFloor);
    rooms.push_back(bossRoom);
    
    currentRoom = rooms[0];
    currentRoomIndex = 0;
    
    int newX = currentRoom->getWidth() / 2;
    int newY = currentRoom->getHeight() / 2;
    player->move(newX, newY);
    currentRoom->placeEntity(*player, newX, newY);
    
    needsRedraw = true;
    printf("\nPrzeszedłeś na piętro %d!\n", currentFloor);
}

void Game::showEnding()
{
    clearScreen();
    printf("=====================================\n");
    printf("         Old Tower Mystery           \n");
    printf("=====================================\n\n");
    
    printf("Po długiej, pełnej niebezpieczeństw podróży dotarłeś do szczytu wieży.\n");
    printf("Pradawny mag, strzegący najwyższego piętra, padł przed Twoją determinacją.\n");
    printf("Tajemnice dawnej wieży, jej zapomniane sekrety i mroczne moce\n");
    printf("wreszcie ujrzały światło dzienne, a Ty stałeś się legendą wśród magów.\n\n");
    
    printf("Koniec gry.\n");
#ifdef _WIN32
    system("pause");
#else
    std::cout << "Naciśnij dowolny klawisz, aby zakończyć..." << std::endl;
    getchWrapper();
#endif
}

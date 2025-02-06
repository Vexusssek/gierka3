#include "Testy.h"

Map* testMap = nullptr;

void Testy::runTests() 
{
    while (true) 
    {
        std::cout << "\n1. Utwórz mapę\n2. Dodaj przedmiot\n3. Dodaj istotę\n";
        std::cout << "4. Test Walki\n5. Test biblioteki potworów\n6. Test generowania pokoi\n7. Wyświetl mapę\n8. Powrót do menu głównego\n";
        
        int choice;
        std::cin >> choice;

        if (choice == 8) break;

        switch (choice) 
        {
            case 1: createMap(); break;
            case 2: addItem(); break;
            case 3: addEntity(); break;
            case 4: testWalki(); break;
            case 5: testMonsters(); break;
            case 6: testRooms(); break;
            case 7: viewMap(); break;
            default:
                std::cout << "Niepoprawny wybór, spróbuj ponownie.\n";
                break;
        }
    }
}

void Testy::createMap() 
{
    int width, height;
    std::cout << "Podaj szerokość i wysokość mapy (bez ścian):\n";
    std::cin >> width >> height;
    delete testMap;
    testMap = new Map(width + 2, height + 2, START_ROOM);
}

bool Testy::isMapFull()
{
    if (!testMap) return true;
    for (int x = 0; x < testMap->getWidth(); ++x) 
    {
        for (int y = 0; y < testMap->getHeight(); ++y) 
        {
            if (testMap->placable(x, y))
                return false;
        }
    }
    return true;
}

void Testy::addItem() 
{
    if (!testMap) 
    {
        std::cout << "Najpierw utwórz mapę!\n";
        return;
    }
    if (isMapFull()) 
    {
        std::cout << "Mapa jest pełna!\n";
        return;
    }
    std::string name;
    char symbol;
    int x, y;
    do 
    {
        std::cout << "Podaj nazwę i symbol przedmiotu oraz pozycję (x y):\n";
        std::cin >> name >> symbol >> x >> y;
        if (!testMap->placable(x, y)) 
        {
            std::cout << "Podana pozycja jest niedostępna. Spróbuj ponownie.\n";
        }
    } while (!testMap->placable(x, y));
    
    Przedmiot item(name, symbol, x, y);
    testMap->placeItem(item, x, y);
}

void Testy::addEntity() 
{
    if (!testMap) 
    {
        std::cout << "Najpierw utwórz mapę!\n";
        return;
    }
    if (isMapFull()) 
    {
        std::cout << "Mapa jest pełna!\n";
        return;
    }
    std::string name;
    char symbol;
    int x, y, hp, attack, defense;
    do 
    {
        std::cout << "Podaj dane istoty (nazwa, symbol, x, y, hp, atak, obrona):\n";
        std::cin >> name >> symbol >> x >> y >> hp >> attack >> defense;
        if (!testMap->placable(x, y)) 
        {
            std::cout << "Podana pozycja jest niedostępna. Spróbuj ponownie.\n";
        }
    } while (!testMap->placable(x, y));
    
    Istota entity(name, symbol, x, y, hp, attack, defense);
    testMap->placeEntity(entity, x, y);
}

void Testy::testWalki() 
{
    Map mapka(4, 4, START_ROOM);
    
    Istota bohater("Bohater", 'H', 2, 2, 20, 6, 3);
    Istota potwor("Potwor", 'M', 2, 1, 15, 4, 2);
    
    mapka.placeEntity(bohater, 2, 2);
    mapka.placeEntity(potwor, 2, 1);

    int wynik = mapka.battle(bohater.getSymbol(), potwor.getSymbol());

    if (wynik == 1)
        std::cout << "Bohater wygrał!\n";
    else if (wynik == -1)
        std::cout << "Potwór wygrał!\n";
    else
        std::cout << "Brak walki\n";
}

void Testy::testMonsters() 
{
    Potwory potwory;
    std::cout << "\nDostępne potwory:\n";
    potwory.printAllMonsters();

    Map mapka(4, 4, START_ROOM);
    char monsterSymbol;
    int istota1WIN = 0, istota2WIN = 0;

    std::cout << "\nWybierz potwora (podaj symbol): ";
    std::cin >> monsterSymbol;

    bool foundMonster = false;
    for (const auto& monster : potwory.getMonsters()) 
    {
        if (monster.getSymbol() == monsterSymbol) 
        {
            foundMonster = true;
            for (int i = 0; i < 5; ++i) 
            {
                Istota hero("Bohater", 'H', 2, 2, 20, 6, 3);
                mapka.placeEntity(hero, 2, 2);
                mapka.placeMonster(monster, 2, 1);

                int wynik = mapka.battle(hero.getSymbol(), monster.getSymbol());
                mapka.removeEntity(2, 2);
                mapka.removeEntity(2, 1);

                if (wynik == 1)
                    istota1WIN++;
                else if (wynik == -1)
                    istota2WIN++;
                else
                    std::cout << "Brak walki\n";
            }
            std::cout << "Bohater wygrał: " << istota1WIN << " razy\n";
            std::cout << "Potwór wygrał: " << istota2WIN << " razy\n";
        }
    }
    if (!foundMonster)
        std::cout << "Nie znaleziono takiego potwora.\n";
}

void Testy::testRooms() 
{
    std::cout << "Test pokoju startowego:\n";
    Map startMap(0, 0, START_ROOM);
    startMap.print();

    std::cout << "Test pokoju standardowego:\n";
    Map standardMap(0, 0, STANDARD_ROOM);
    standardMap.print();

    std::cout << "Test sklepu:\n";
    Map shopMap(0, 0, SHOP_ROOM);
    shopMap.print();

    std::cout << "Test pokoju z bossem:\n";
    Map bossMap(0, 0, BOSS_ROOM);
    bossMap.print();
}

void Testy::viewMap() 
{
    if (!testMap) 
    {
        std::cout << "Najpierw utwórz mapę!\n";
        return;
    }
    testMap->print();
}

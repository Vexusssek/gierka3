#ifndef TESTY_H
#define TESTY_H

#include "Map.h"
#include "Potwory.h"
#include <iostream>

class Testy {
public:
    void runTests();
private:
    void testWalki();
    void testMonsters();
    void testRooms();
    void createMap();
    void addItem();
    void addEntity();
    void viewMap();
    bool isMapFull();

};

#endif

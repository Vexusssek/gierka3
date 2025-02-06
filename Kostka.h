#pragma once
#include <cstdlib>
#include <ctime>

class Kostka
{
private:
    int liczbaScianek;
public:
    Kostka(int liczbaScianek) : liczbaScianek(liczbaScianek) {}
    int getLiczbaScianek() const { return liczbaScianek;}
    int rzut();
};
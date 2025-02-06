#pragma once
#include "Przedmiot.h"
#include <vector>
#include <string>

class Ekwipunek 
{
private:
    std::vector<Przedmiot> items;
    static const int MAX_ITEMS = 10;

public:
    bool addItem(const Przedmiot& item);
    void removeItem(int index);
    void display() const;
    bool isFull() const { return items.size() >= MAX_ITEMS; }
    const std::vector<Przedmiot>& getItems() const { return items; }
    void getDisplayData(std::vector<std::string>& output) const;
};
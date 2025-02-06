#include "Ekwipunek.h"
#include <cstdio>

bool Ekwipunek::addItem(const Przedmiot& item) 
{
    for (const auto& existingItem : items) 
    {
        if (existingItem.getName() == item.getName()) 
        {
            return false;
        }
    }
    if (items.size() < MAX_ITEMS) 
    {
        items.push_back(item);
        return true;
    }
    return false;
}

void Ekwipunek::removeItem(int index) 
{
    if (index >= 0 && index < items.size()) 
    {
        items.erase(items.begin() + index);
    }
}

void Ekwipunek::display() const 
{
    printf("\nEkwipunek:\n");
    if(items.empty()) 
    {
        printf("(pusty)\n");
    } 
    else 
    {
        for (size_t i = 0; i < items.size(); i++) 
        {
            printf("%c %s\n", items[i].getSymbol(), items[i].getName().c_str());
        }
    }
}
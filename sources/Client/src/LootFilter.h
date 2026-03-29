#include "stdafx.h"
#include <map>

class LootFilter {
public:
    LootFilter();
    ~LootFilter();

    std::map<int, bool> GetFilteredItems() { return lootFilter; }
    void AddFilteredItem(unsigned long itemId);
    void RemoveFilteredItem(unsigned long itemId);
    bool HasFilteredItem(unsigned long itemId);
    void PrintAllItems();
private:
    std::map<int, bool> lootFilter;
};


extern LootFilter* g_lootFilter;

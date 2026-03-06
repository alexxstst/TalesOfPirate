#include "stdafx.h"
#include "LootFilter.h"
#include "GameApp.h"
#include <string>

using namespace std;

LootFilter::LootFilter() {
    if (!CreateDirectory("user", NULL)) {
        DWORD dw = GetLastError();
        if (dw != 183) {
            return;
        }
    }

    ifstream lootFilterFile;
    lootFilterFile.open("user\\lootfilter.txt");

    std::string itemIdReader;
    if (lootFilterFile.is_open()) {
        while (getline(lootFilterFile, itemIdReader)) {
            if (itemIdReader.length() == 0) {
                continue;
            }
            int itemId = atoi(itemIdReader.c_str());
            lootFilter.insert(pair<int, bool>(itemId, TRUE));
        }
    }
}

void LootFilter::AddFilteredItem(unsigned long itemId) {
    lootFilter.insert(pair<int, bool>(itemId, TRUE));

    ofstream lootFilterFile;
    char itemIdBuffer[10];
    _itoa(itemId, itemIdBuffer, 10);
    lootFilterFile.open("user\\lootfilter.txt", ios_base::app);
    lootFilterFile << string(itemIdBuffer) << std::endl;
    lootFilterFile.close();
}

void LootFilter::RemoveFilteredItem(unsigned long itemId) {
    lootFilter.erase(itemId);
    string line;
    ifstream in;
    in.open("user\\lootfilter.txt");
    ofstream temp;
    temp.open("user\\lootfilter_new.txt");

    while (getline(in, line)) {
        char itemIdBuffer[10];
        _itoa(itemId, itemIdBuffer, 10);
        if (line.compare(string(itemIdBuffer)) != 0) {
            temp << line << std::endl;
        }
    }

    in.close();
    temp.close();

    remove("user\\lootfilter.txt");
    rename("user\\lootfilter_new.txt", "user\\lootfilter.txt");
}

bool LootFilter::HasFilteredItem(unsigned long itemId) {
    if (lootFilter.find(itemId) == lootFilter.end()) {
        return false;
    }

    return true;
}

void LootFilter::PrintAllItems() {
    auto it = lootFilter.begin();
    for (it = lootFilter.begin(); it != lootFilter.end(); it++) {
        g_pGameApp->SysInfo("Filtered item %d %d", it->first, it->second);
    }
}

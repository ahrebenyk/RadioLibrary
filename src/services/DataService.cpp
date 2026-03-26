#include "services/DataService.h"
#include <fstream>
#include <iostream>
#include "services/JsonFileService.h"

using namespace std;

DataService::DataService(string dbFile) : filename(dbFile) {
}

void DataService::clear() {
    database.clear();
}

void DataService::load() {
    database = JsonFileService::loadFromFile(this->filename);
    if (database.empty()) {
        cout << "База даних порожня або файл не знайдено." << endl;
    }
}

void DataService::print() {
    for (const auto& item : database) {
        item->showInfo();
        cout << endl;
    }
}

void DataService::printByType(ComponentType type) {
    for (const auto& item : database) {
        if (item->getType() == type) {
            item->showInfo();
            cout << endl;
        }
    }
}

Component* DataService::getById(int targetId) {
    for (const auto& item : database) {
        if (item->getId() == targetId) {
            return item.get();
        }
    }
    return nullptr;
}

vector<Component*> DataService::searchByName(const string& namePart) {
    vector<Component*> results;

    string query = namePart;
    ranges::transform(query, query.begin(), ::tolower);

    for (const auto& item : database) {
        string itemName = item->getName();
        ranges::transform(itemName, itemName.begin(), ::tolower);

        if (itemName.find(query) != string::npos) {
            results.push_back(item.get());
        }
    }
    return results;
}
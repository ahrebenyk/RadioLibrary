#include "DataService.h"
#include <fstream>
#include <iostream>

#include "JsonFileService.h"

using namespace std;

DataService::DataService(string dbFile) : filename(dbFile) {
}

void DataService::clear() {
    database.clear();
}

void DataService::load() {
    database = JsonFileService::loadFromFile(this->filename);
    if (database.empty()) {
        std::cout << "База даних порожня або файл не знайдено." << endl;
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
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

vector<Component*> DataService::getAll() {
    vector<Component*> all;
    all.reserve(database.size());

    for (const auto& item : database) {
        all.push_back(item.get());
    }

    return all;
}

Component* DataService::getById(int targetId) {
    for (const auto& item : database) {
        if (item->getId() == targetId) {
            return item.get();
        }
    }
    return nullptr;
}

vector<Component*> DataService::searchByType(const ComponentType type) {
    std::vector<Component*> results;
    for (const auto& item : database) {
        if (item->getType() == type) {
            results.push_back(item.get());
        }
    }
    return results;
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

bool DataService::deleteById(int id) {
    auto it = ranges::find_if(
        database,
        [id](const unique_ptr<Component>& item) {
            return item->getId() == id;
    });

    if (it != database.end()) {
        database.erase(it);
        JsonFileService::saveToFile(this->filename, database);
        return true;
    }

    return false;
}
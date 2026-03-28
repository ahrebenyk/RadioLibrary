#ifndef RADIOLIBRARY_DATASERVICE_H
#define RADIOLIBRARY_DATASERVICE_H
#include <vector>
#include <string>
#include "components/Component.h"
#include <memory>
#include "external/json.hpp"

using namespace std;

using json = nlohmann::json;

class DataService {
private:
    vector<unique_ptr<Component>> database;
    string filename;
public:
    DataService(string dbFile);
    ~DataService() = default;
    void load();
    Component* getById(int targetId);
    vector<Component*> searchByType(ComponentType type);
    void save();
    vector<Component*> getAll();
    vector<Component*> searchByName(const string& namePart);
    bool deleteById(int id);
    void add(unique_ptr<Component> component);
    int getNextId() const;
    void clear();
};
#endif
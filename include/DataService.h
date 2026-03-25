#ifndef RADIOLIBRARY_DATASERVICE_H
#define RADIOLIBRARY_DATASERVICE_H
#include <vector>
#include <string>
#include "Component.h"
#include <memory>
#include <json.hpp>

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
    void save();
    void add(Component* c);
    void print();
    void printByType(ComponentType type);
    void clear();
};
#endif
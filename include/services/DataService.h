#ifndef RADIOLIBRARY_DATASERVICE_H
#define RADIOLIBRARY_DATASERVICE_H
#include <vector>
#include <string>
#include "components/Component.h"
#include <memory>
#include "external/json.hpp"
#include "users/User.h"

using namespace std;

using json = nlohmann::json;

class DataService {
private:
    shared_ptr<User> currentUser;
    vector<unique_ptr<Component>> database;
    string filename;
public:
    DataService(string dbFile, shared_ptr<User> user);
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
    void checkAdminAccess();
    void clear();
};
#endif
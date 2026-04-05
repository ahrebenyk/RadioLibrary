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

const string DATASERVICE_ERROR_COLOR = "\033[31m";
const string DATASERVICE_RESET_COLOR = "\033[0m";

class DataService {
private:
    shared_ptr<User> currentUser;
    vector<unique_ptr<Component>> database;
    string filename;
    Component* getByIdInternal(int targetId) const;
    void save();
    void add(unique_ptr<Component> component);
public:
    DataService(const string& dbFile, shared_ptr<User> user);
    ~DataService() = default;
    void load();
    const Component* getById(int targetId) const;
    bool existsByName(const string& name) const;
    vector<const Component*> search(optional<int> targetId, optional<ComponentType> type, const optional<string>& namePart) const;
    bool deleteById(int id);
    void updateResistor(int id, optional<string> name, optional<double> resistance, optional<double> power);
    void updateDiode(int id, optional<string> name, optional<double> current, optional<double> voltage,
                     optional<string> material);
    void updateTransistor(int id, optional<string> name, optional<string> polatiry, optional<double> voltage,
                          optional<double> current, optional<double> gain);
    void updateCapacitor(int id, optional<string> name, optional<double> voltage, optional<double> capacity);
    void addResistor(const string&, double res, double power);
    int getNextId() const;
    void checkAdminAccess() const;
    void addDiode(const string& name, double volt, double curr, const string& mat);
    void addTransistor(const string& name, const string& polarity, double volt, double curr, double gain);
    void addCapacitor(const string& name, double volt, double capacity);
    static void printEditError();
    void clear();
};
#endif
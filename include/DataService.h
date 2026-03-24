#ifndef RADIOLIBRARY_DATASERVICE_H
#define RADIOLIBRARY_DATASERVICE_H
#include <vector>
#include <string>
#include "Component.h"
#include "Resistor.h"
#include "Diode.h"
#include "Transistor.h"
#include "Capacitor.h"
#include <memory>
#include <json.hpp>

using namespace std;

using json = nlohmann::json;

class DataService {
private:
    vector<unique_ptr<Component>> database;
    string filename;
    unique_ptr<Resistor> parseResistor(const json& j);
    unique_ptr<Diode> parseDiode(const json& j);
    unique_ptr<Transistor> parseTransistor(const json& j);
    unique_ptr<Capacitor> parseCapacitor(const json& j);
public:
    DataService(string dbFile);
    ~DataService() = default;
    void load();
    void save();
    void add(Component* c);
    void print();
    void printByType(ComponentType type);
    void clear();
};
#endif
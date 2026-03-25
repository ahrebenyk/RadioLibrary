#ifndef RADIOLIBRARY_JSONSERVICE_H
#define RADIOLIBRARY_JSONSERVICE_H
#include <vector>
#include <string>
#include "Component.h"
#include <memory>
#include <json.hpp>
#include "Component.h"
#include "Resistor.h"
#include "Diode.h"
#include "Transistor.h"
#include "Capacitor.h"

using namespace std;

using json = nlohmann::json;

class JsonFileService {
public:
    static vector<unique_ptr<Component>> loadFromFile(const string& filename);
    static void saveToFile(const string& filename, const vector<unique_ptr<Component>>& data);

private:
    static unique_ptr<Component> parseComponent(const json& j);
    static unique_ptr<Resistor> parseResistor(const json& j);
    static unique_ptr<Capacitor> parseCapacitor(const json& j);
    static unique_ptr<Diode> parseDiode(const json& j);
    static unique_ptr<Transistor> parseTransistor(const json& j);
};

#endif
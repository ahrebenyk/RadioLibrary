#ifndef RADIOLIBRARY_JSONSERVICE_H
#define RADIOLIBRARY_JSONSERVICE_H
#include <vector>
#include <string>
#include <memory>
#include "external/json.hpp"
#include "components/Component.h"
#include "components/Resistor.h"
#include "components/Diode.h"
#include "components/Transistor.h"
#include "components/Capacitor.h"

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
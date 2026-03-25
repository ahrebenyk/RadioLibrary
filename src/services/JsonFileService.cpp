#include "services/JsonFileService.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<unique_ptr<Component>> JsonFileService::loadFromFile(const string& filename) {
    vector<unique_ptr<Component>> components;
    ifstream file(filename);

    if (!file.is_open()) return components;

    json jData;
    try {
        file >> jData;
        for (const auto& item : jData) {
            auto comp = parseComponent(item);
            if (comp) components.push_back(std::move(comp));
        }
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON Error: " << e.what() << std::endl;
    }

    return components;
}

unique_ptr<Component> JsonFileService::parseComponent(const json& j) {
    string type = j.at("type").get<string>();
    if (type == "Resistor") {
        return parseResistor(j);
    }
    if (type == "Diode") {
        return parseDiode(j);
    }
    if (type == "Transistor") {
        return parseTransistor(j);
    }
    if (type == "Capacitor") {
        return parseCapacitor(j);
    }
    cout << "Unknown type " << type << endl;
    return nullptr;
}

unique_ptr<Resistor> JsonFileService::parseResistor(const json& j) {
    return make_unique<Resistor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("resistance").get<double>(),
        j.at("power").get<double>()
    );
}

unique_ptr<Capacitor> JsonFileService::parseCapacitor(const json& j) {
    return make_unique<Capacitor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("voltage").get<double>(),
        j.at("capacity").get<double>()
    );
}

unique_ptr<Diode> JsonFileService::parseDiode(const json& j) {
    return make_unique<Diode>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("voltage").get<double>(),
        j.at("current").get<double>(),
        j.at("material").get<string>()
    );
}

unique_ptr<Transistor> JsonFileService::parseTransistor(const json& j) {
    string tStr = j.at("polarity").get<string>();
    TransistorType tEnum = (tStr == "PNP") ? TransistorType::PNP : TransistorType::NPN;

    return make_unique<Transistor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        tEnum,
        j.at("voltage").get<double>(),
        j.at("current").get<double>(),
        j.at("gain").get<int>()
    );
}

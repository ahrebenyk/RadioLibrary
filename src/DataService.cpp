#include "DataService.h"
#include <fstream>
#include <iostream>

using namespace std;

DataService::DataService(string dbFile) : filename(dbFile) {
}

void DataService::clear() {
    database.clear();
}

void DataService::load() {
    ifstream file(filename);
    if (!file.is_open()) return;

    json jData;
    try {
        file >> jData;
    } catch (json::parse_error& e) {
        cerr << "Помилка парсингу JSON: " << e.what() << std::endl;
        return;
    }

    clear();

    for (const auto& item : jData) {
        string type = item.at("type").get<string>();
        if (type == "Resistor") {
            database.push_back(parseResistor(item));
        } else if (type == "Diode") {
            database.push_back(parseDiode(item));
        } else if (type == "Transistor") {
            database.push_back(parseTransistor(item));
        } else if (type == "Capacitor") {
            database.push_back(parseCapacitor(item));
        }
    }
}
unique_ptr<Resistor> DataService::parseResistor(const json& j) {
    return make_unique<Resistor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("resistance").get<double>(),
        j.at("power").get<double>()
    );
}

unique_ptr<Capacitor> DataService::parseCapacitor(const json& j) {
    return make_unique<Capacitor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("voltage").get<double>(),
        j.at("capacity").get<double>()
    );
}

unique_ptr<Diode> DataService::parseDiode(const json& j) {
    return make_unique<Diode>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("voltage").get<double>(),
        j.at("current").get<double>(),
        j.at("material").get<string>()
    );
}

unique_ptr<Transistor> DataService::parseTransistor(const json& j) {
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

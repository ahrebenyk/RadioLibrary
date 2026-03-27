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
    } catch (const json::parse_error& e) {
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
    return make_unique<Transistor>(
        j.at("id").get<int>(),
        j.at("name").get<string>(),
        j.at("polarity").get<string>(),
        j.at("voltage").get<double>(),
        j.at("current").get<double>(),
        j.at("gain").get<int>()
    );
}

void JsonFileService::saveToFile(const std::string& filename, const std::vector<std::unique_ptr<Component>>& database) {
    nlohmann::json jArray = nlohmann::json::array();

    for (const auto& item : database) {
        nlohmann::json jItem;
        jItem["id"] = item->getId();
        jItem["name"] = item->getName();
        jItem["type"] = componentTypeToString(item->getType());

        if (item->getType() == ComponentType::Resistor) {
            auto* r = static_cast<Resistor*>(item.get());
            jItem["resistance"] = r->getResistance();
            jItem["power"] = r->getPower();
        } else if (item->getType() == ComponentType::Transistor) {
            auto* t = static_cast<Transistor*>(item.get());
            jItem["gain"] = t->getGain();
            jItem["polarity"] = t->getPolarity();
            jItem["current"] = t->getCurrent();
            jItem["volatage"] = t->getVoltage();
        } else if (item->getType() == ComponentType::Diode) {
            auto* t = static_cast<Diode*>(item.get());
            jItem["current"] = t->getCurrent();
            jItem["voltage"] = t->getVoltage();
            jItem["material"] = t->getMaterial();
        } else if (item->getType() == ComponentType::Capacitor) {
            auto* t = static_cast<Capacitor*>(item.get());
            jItem["capacity"] = t->getCapacity();
            jItem["voltage"] = t->getVoltage();
        }

        jArray.push_back(jItem);
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << jArray.dump(4);
        file.close();
    }
}

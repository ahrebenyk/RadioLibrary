#include "services/DataService.h"
#include <fstream>
#include <iostream>
#include "services/JsonStorageService.h"

using namespace std;

DataService::DataService(const string& dbFile, shared_ptr<User> user) : filename(dbFile), currentUser(user) {
}

void DataService::clear() {
    database.clear();
}

void DataService::load() {
    database = JsonStorageService::loadFromFile(this->filename);
    if (database.empty()) {
        cout << "База даних порожня або файл не знайдено." << endl;
    }
}

vector<const Component*> DataService::getAll() const {
    vector<const Component*> all;
    all.reserve(database.size());

    for (const auto& item : database) {
        all.push_back(item.get());
    }

    return all;
}

const Component* DataService::getById(int targetId) const {
    for (const auto& item : database) {
        if (item->getId() == targetId) {
            return item.get();
        }
    }
    return nullptr;
}

Component* DataService::getByIdInternal(int targetId) const {
    for (auto& item : database) {
        if (item->getId() == targetId) {
            return item.get();
        }
    }
    return nullptr;
}

vector<const Component*> DataService::searchByType(const ComponentType type) const {
    std::vector<const Component*> results;
    for (const auto& item : database) {
        if (item->getType() == type) {
            results.push_back(item.get());
        }
    }
    return results;
}

vector<const Component*> DataService::searchByName(const string& namePart) const {
    vector<const Component*> results;

    string query = namePart;
    ranges::transform(query, query.begin(), ::tolower);

    for (const auto& item : database) {
        string itemName = item->getName();
        ranges::transform(itemName, itemName.begin(), ::tolower);

        if (itemName.find(query) != string::npos) {
            results.push_back(item.get());
        }
    }
    return results;
}

bool DataService::deleteById(int id) {
    checkAdminAccess();
    auto it = ranges::find_if(
        database,
        [id](const unique_ptr<Component>& item) {
            return item->getId() == id;
        });

    if (it != database.end()) {
        database.erase(it);
        JsonStorageService::saveToFile(this->filename, database);
        return true;
    }

    return false;
}

void DataService::updateResistor(int id, optional<string> name, optional<double> resistance, optional<double> power) {
    checkAdminAccess();
    Component* comp = getByIdInternal(id);
    if (!comp) {
        printEditError();
        return;
    }
    auto* resistor = dynamic_cast<Resistor*>(comp);
    if (!resistor) {
        printEditError();
        return;
    }
    if (name.has_value()) {
        resistor->setName(name.value());
    }
    if (resistance.has_value()) {
        resistor->setResistance(resistance.value());
    }
    if (power.has_value()) {
        resistor->setPower(power.value());
    }
    save();
}

void DataService::updateDiode(int id, optional<string> name, optional<double> current,
                              optional<double> voltage, optional<string> material) {
    checkAdminAccess();
    Component* comp = getByIdInternal(id);
    if (!comp) {
        printEditError();
        return;
    }
    auto* diode = dynamic_cast<Diode*>(comp);
    if (!diode) {
        printEditError();
        return;
    }
    if (name.has_value()) {
        diode->setName(name.value());
    }
    if (current.has_value()) {
        diode->setCurrent(current.value());
    }
    if (voltage.has_value()) {
        diode->setVoltage(voltage.value());
    }
    if (material.has_value()) {
        diode->setMaterial(material.value());
    }
    save();
}

void DataService::updateTransistor(int id, optional<string> name, optional<string> polatiry, optional<double> voltage,
                                   optional<double> current, optional<double> gain) {
    checkAdminAccess();
    Component* comp = getByIdInternal(id);
    if (!comp) {
        printEditError();
        return;
    }
    auto* transistor = dynamic_cast<Transistor*>(comp);
    if (!transistor) {
        printEditError();
        return;
    }
    if (name.has_value()) {
        transistor->setName(name.value());
    }
    if (polatiry.has_value()) {
        transistor->setPolarity(polatiry.value());
    }
    if (voltage.has_value()) {
        transistor->setVoltage(voltage.value());
    }
    if (current.has_value()) {
        transistor->setCurrent(current.value());
    }
    if (gain.has_value()) {
        transistor->setGain(gain.value());
    }
    save();
}

void DataService::updateCapacitor(int id, optional<string> name, optional<double> voltage, optional<double> capacity) {
    checkAdminAccess();
    Component* comp = getByIdInternal(id);
    if (!comp) {
        printEditError();
        return;
    }
    auto* capacitor = dynamic_cast<Capacitor*>(comp);
    if (!capacitor) {
        printEditError();
        return;
    }
    if (name.has_value()) {
        capacitor->setName(name.value());
    }
    if (voltage.has_value()) {
        capacitor->setVoltage(voltage.value());
    }
    if (capacity.has_value()) {
        capacitor->setCapacity(capacity.value());
    }
    save();
}

void DataService::addResistor(const string& name, double res, double power) {
    checkAdminAccess();
    auto resistor = make_unique<Resistor>(getNextId(), name, res, power);
    add(std::move(resistor));
}

void DataService::addDiode(const string& name, double volt, double curr, const string& mat) {
    checkAdminAccess();
    auto diode = make_unique<Diode>(getNextId(), name, volt, curr, mat);
    add(std::move(diode));
}

void DataService::addTransistor(const string& name, const string& polarity, double volt, double curr, double gain) {
    checkAdminAccess();
    auto transistor = make_unique<Transistor>(getNextId(), name, polarity, volt, curr, gain);
    add(std::move(transistor));
}

void DataService::addCapacitor(const string& name, double volt, double capacity) {
    checkAdminAccess();
    auto capacitor = make_unique<Capacitor>(getNextId(), name, volt, capacity);
    add(std::move(capacitor));
}

void DataService::add(unique_ptr<Component> component) {
    if (component != nullptr) {
        database.push_back(std::move(component));
        save();
    }
}

void DataService::save() {
    checkAdminAccess();
    try {
        JsonStorageService::saveToFile(this->filename, database);
    } catch (const exception& e) {
        cout << "Помилка під час збереження даних: " << e.what() << endl;
    }
}

int DataService::getNextId() const {
    if (database.empty()) {
        return 1;
    }

    return database.back()->getId() + 1;
}

void DataService::checkAdminAccess() const {
    if (!currentUser->isAdmin()) {
        throw std::runtime_error("Відмовлено в доступі!");
    }
}

void DataService::printEditError() {
    cout << DATASERVICE_ERROR_COLOR;
    cout << "Помилка редагування компонента\n";
    cout << DATASERVICE_RESET_COLOR;
}
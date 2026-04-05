#include <iostream>
#include <sstream>
#include <format>

#include "services/CLI.h"

#include "components/Component.h"
#include "components/Resistor.h"
#include "components/Diode.h"
#include "components/Transistor.h"
#include "components/Capacitor.h"

using namespace std;

CLI::CLI(DataService& dataService, shared_ptr<User> user) : ds(dataService), currentUser(user) {
}

void CLI::run() {
    printInfo("Введіть команду або 'help' для підказки");
    string line;
    while (true) {
        printPrompt();
        if (!getline(cin, line)) break;
        const auto commandData = parseCommand(line);
        if (line.empty()) continue;
        if (line == "quit" || line == "exit" || line == "q") break;
        runCommand(commandData);
    }
}

void CLI::runCommand(const CommandData& cmd) {
    auto action = cmd.action;
    if (action.empty()) return;

    try {
        if (action == "list" || action == "ls" || action == "search" || action == "s" ) {
            listComponents(cmd);
        } else if (action == "add" || action == "a") {
            addComponent(cmd);
        } else if (action == "edit" || action == "e") {
            editComponent(cmd);
        } else if (action == "delete" || action == "del" || action == "d") {
            deleteComponent(cmd);
        } else if (action == "help" || action == "h") {
            showHelp();
        } else if (action == "su") {
            switchUser(cmd);
        } else {
            printError(format("Невідома команда '{}'. Введіть 'help' для довідки", action));
        }
    } catch (const exception& e) {
        printError(format("Помилка: {}", e.what()));
    }
}

CommandData CLI::parseCommand(const string& input) {
    stringstream ss(input);
    CommandData data;

    if (!(ss >> data.action)) return data;

    string key, value;
    while (ss >> key) {
        if (ss >> quoted(value)) {
            data.args[key] = value;
        }
    }
    return data;
}

void CLI::listComponents(const CommandData& cmd) const {
    auto compTypeStr = getOptionalStringArg(cmd.args, "--type", "--t");
    auto id = getOptionalIntArg(cmd.args, "--id");
    auto namePart = getOptionalStringArg(cmd.args, "--name", "--n");
    optional<ComponentType> compTypeOpt;
    if (compTypeStr) {
        compTypeOpt = strToCompType(compTypeStr.value());
        if (!compTypeOpt) return;
    }

    auto results = ds.search(id, compTypeOpt, namePart);
    printInfo(format("Знайдено компонентів: {}", results.size()));
    showComponents(results);
}

void CLI::addComponent(const CommandData& cmd) {
    if (!checkAccess()) return;

    auto typeStr = getRequiredStringArg(cmd.args, "--type", "--t");
    auto name = getRequiredStringArg(cmd.args, "--name", "--n");

    if (!typeStr || !name) return;
    auto compType = strToCompType(typeStr.value());

    if (!compType) return;
    if (ds.existsByName(name.value())) {
        printError(format("Компонент з назвою {} вже існує", name.value()));
        return;
    }

    switch (compType.value()) {
    case ComponentType::Resistor: {
        auto res = getRequiredDoubleArg(cmd.args, "--resistance", "--r");
        auto pwr = getRequiredDoubleArg(cmd.args, "--power", "--p");
        if (!res || !pwr) return;
        ds.addResistor(name.value(), res.value(), pwr.value());
        break;
    }
    case ComponentType::Diode: {
        auto volt = getRequiredDoubleArg(cmd.args, "--voltage", "--v");
        auto curr = getRequiredDoubleArg(cmd.args, "--current", "--c");
        auto mat = getRequiredStringArg(cmd.args, "--material", "--m");
        if (!volt || !curr || !mat) return;
        ds.addDiode(name.value(), volt.value(), curr.value(), mat.value());
        break;
    }
    case ComponentType::Transistor: {
        auto volt = getRequiredDoubleArg(cmd.args, "--voltage", "--v");
        auto curr = getRequiredDoubleArg(cmd.args, "--current", "--c");
        auto gain = getRequiredDoubleArg(cmd.args, "--gain", "--g");
        auto pol = getRequiredStringArg(cmd.args, "--polarity", "--p");
        if (!volt || !curr || !gain || !pol) return;
        ds.addTransistor(name.value(), pol.value(), volt.value(), curr.value(), gain.value());
        break;
    }
    case ComponentType::Capacitor: {
        auto volt = getRequiredDoubleArg(cmd.args, "--voltage", "--v");
        auto cap = getRequiredDoubleArg(cmd.args, "--capacity", "--c");
        if (!volt || !cap) return;
        ds.addCapacitor(name.value(), volt.value(), cap.value());
        break;
    }
    }
    printInfo(format("Компонент '{}' успішно додано", name.value()));
}

void CLI::editComponent(const CommandData& cmd) {
    if (!checkAccess()) return;
    auto args = cmd.args;
    auto id = getRequiredIntArg(args, "--id");
    if (!id) return;

    const Component* comp = ds.getById(id.value());
    if (!comp) {
        printError(format("Компонент з ID {} не знайдено", id.value()));
        return;
    }

    switch (comp->getType()) {
    case ComponentType::Resistor: {
        auto* r = dynamic_cast<const Resistor*>(comp);
        auto n = getOptionalStringArg(args, "--name", "--n");
        auto res = getOptionalDoubleArg(args, "--resistance", "--r");
        auto pwr = getOptionalDoubleArg(args, "--power", "--p");
        if (!n && !res && !pwr) {
            printError("Вкажіть хоча б одне поле: --name, --resistance, --power");
            return;
        }
        ds.updateResistor(r->getId(), n, res, pwr);
        break;
    }
    case ComponentType::Diode: {
        auto* d = dynamic_cast<const Diode*>(comp);
        auto n = getOptionalStringArg(args, "--name", "--n");
        auto volt = getOptionalDoubleArg(args, "--voltage", "--v");
        auto curr = getOptionalDoubleArg(args, "--current", "--c");
        auto mat = getOptionalStringArg(args, "--material", "--m");
        if (!n && !volt && !curr && !mat) {
            printError("Вкажіть хоча б одне поле: --name, --voltage, --current, --material");
            return;
        }
        ds.updateDiode(d->getId(), n, volt, curr, mat);
        break;
    }
    case ComponentType::Transistor: {
        auto* t = dynamic_cast<const Transistor*>(comp);
        auto n = getOptionalStringArg(args, "--name", "--n");
        auto pol = getOptionalStringArg(args, "--polarity", "--p");
        auto volt = getOptionalDoubleArg(args, "--voltage", "--v");
        auto curr = getOptionalDoubleArg(args, "--current", "--c");
        auto gain = getOptionalDoubleArg(args, "--gain", "--g");
        if (!n && !pol && !volt && !curr && !gain) {
            printError("Вкажіть хоча б одне поле: --name, --polarity, --voltage, --current, --gain");
            return;
        }
        ds.updateTransistor(t->getId(), n, pol, volt, curr, gain);
        break;
    }
    case ComponentType::Capacitor: {
        auto* cap = dynamic_cast<const Capacitor*>(comp);
        auto n = getOptionalStringArg(args, "--name", "--n");
        auto volt = getOptionalDoubleArg(args, "--voltage", "--v");
        auto cval = getOptionalDoubleArg(args, "--capacity", "--c");
        if (!n && !volt && !cval) {
            printError("Вкажіть хоча б одне поле: --name, --voltage, --capacity");
            return;
        }
        ds.updateCapacitor(cap->getId(), n, volt, cval);
        break;
    }
    }

    printInfo(format("Компонент ID {} змінено", id.value()));
}

void CLI::deleteComponent(const CommandData& cmd) {
    if (!checkAccess()) return;
    auto idOpt = getRequiredIntArg(cmd.args, "--id");
    if (!idOpt) return;
    int id = idOpt.value();
    const Component* comp = ds.getById(id);
    if (!comp) {
        printError(format("Компонент з ID {} не знайдено", id));
        return;
    }
    showComponent(comp);

    if (!confirm(format("Видалити компонент ID {}?", id))) {
        printInfo("Видалення скасовано");
        return;
    }

    if (ds.deleteById(id)) {
        printInfo(format("Компонент ID {} видалено", id));
        return;
    }
    printError("Не вдалося видалити");
}

void CLI::switchUser(const CommandData& cmd) {
    auto role = getRequiredStringArg(cmd.args, "--user", "--u");
    if (!role) return;

    if (role.value() == "admin") {
        currentUser->setRole(UserRole::ADMIN);
        return;
    }
    if (role.value() == "guest") {
        currentUser->setRole(UserRole::GUEST);
        return;
    }
    printError(format("Неправильна роль '{}'", role.value()));
}

void CLI::showHelp() {
    showViewCommandsHelp();
    showEditCommandsHelp();
}

void CLI::showViewCommandsHelp() {
    cout << CLI_INF_CLR << "Змінити користувача:" << CLI_RESET_CLR << "\n";
    cout << "su --u <role>          Можливі значення: admin, guest\n";
    cout << CLI_INF_CLR << "Команди перегляду:" << CLI_RESET_CLR << "\n";
    cout << "list, ls, search, s    Пошук компонентів\n";
    cout << "  --type (--t) <t>     Опціональний фільтр за типом: resistor, diode, transistor, capacitor\n\n";
    cout << "  --name (--n) <name>  Опціональний фільтр за назвою або частиною назви\n\n";
    cout << "  --id <id>            Опціональний фільтр за id\n\n";
    cout << CLI_INF_CLR << "Приклади:" << CLI_RESET_CLR << "\n";
    cout << CLI_EXAMPLES_CLR;
    cout << "list\n";
    cout << "ls --type resistor\n";
    cout << "list --t resistor\n";
    cout << "list --t r\n";
    cout << "ls --name BC\n";
    cout << "ls --id 3\n";
    cout << CLI_RESET_CLR << "\n";
}

void CLI::showEditCommandsHelp() {
    cout << CLI_INF_CLR << "Команди редагування, потребують прав адміністратора:" << CLI_RESET_CLR << "\n";
    cout << "add, a                 Додати компонент\n";
    cout << "  --type (--t) <t>     Тип компонента\n";
    cout << "  --name (--n) <назва> Назва\n";
    cout << "  Резистор:      --resistance (--r) <Ом>  --power (--p) <Вт>\n";
    cout << "  Діод:          --voltage (--v) <В>  --current (--c) <А>  --material (--m) <матеріал>\n";
    cout << "  Транзистор:    --polarity (--p)  --voltage (--v) <В>  --current (--c) <А>  --gain (--g)\n";
    cout << "  Конденсатор:   --voltage (--v) <В>  --capacity (--c) <мФ>\n\n";
    cout << "edit, e                Редагувати компонент (тільки передані поля)\n";
    cout << "  --id <id>            Ідентифікатор\n";
    cout << "  [будь-які поля з команди add]\n\n";
    cout << "delete, del, d         Видалити компонент\n";
    cout << "  --id <id>            Ідентифікатор\n";
    cout << CLI_INF_CLR << "Приклади:" << CLI_RESET_CLR << "\n";
    cout << CLI_EXAMPLES_CLR;
    cout << "  add --type diode --name D4148 --voltage 75 --current 0.3 --material Si\n";
    cout << "  add --t diode --n D4148 --v 75 --c 0.3 --m Si\n";
    cout << "  edit --id 3 --resistance 2200\n";
    cout << "  delete --id 3\n";
    cout << CLI_RESET_CLR << "\n";
}

optional<ComponentType> CLI::strToCompType(const string& typeStr) {
    if (typeStr == "resistor" || typeStr == "r") return ComponentType::Resistor;
    if (typeStr == "diode" || typeStr == "d") return ComponentType::Diode;
    if (typeStr == "transistor" || typeStr == "t") return ComponentType::Transistor;
    if (typeStr == "capacitor" || typeStr == "c") return ComponentType::Capacitor;
    printInfo(format("Невідомий тип '{}'. Допустимі: resistor (r), diode (d), transistor (t), capacitor (c)", typeStr));
    return nullopt;
}

optional<int> CLI::getRequiredIntArg(const map<string, string>& args, const string& key) {
    return getIntArg(args, key, true);
}

optional<int> CLI::getOptionalIntArg(const map<string, string>& args, const string& key) {
    return getIntArg(args, key, false);
}

optional<int> CLI::getIntArg(const map<string, string>& args, const string& key, bool required) {
    if (!args.contains(key)) {
        if (required) printError(format("Додайте параметр {}", key));
        return nullopt;
    }
    return parseInt(key, args.at(key));
}

optional<double> CLI::getRequiredDoubleArg(const map<string, string>& args, const string& key, const string& altKey) {
    return getDoubleArg(args, key, altKey, true);
}

optional<double> CLI::getOptionalDoubleArg(const map<string, string>& args, const string& key, const string& altKey) {
    return getDoubleArg(args, key, altKey, false);
}

optional<double> CLI::getDoubleArg(const map<string, string>& args, const string& key, const string& altKey,
                                   bool required) {
    if (!checkArg(args, key, altKey, required)) {
        return nullopt;
    }
    if (args.contains(key)) {
        return parseDouble(key, args.at(key));
    }
    return parseDouble(altKey, args.at(altKey));
}

optional<double> CLI::parseDouble(const string& key, const string& value) {
    try {
        return stod(value);
    } catch (...) {
        printError(format("Невірне значення для {}: '{}'. Введіть число (напр. 1.23)", key, value));
        return nullopt;
    }
}

optional<int> CLI::parseInt(const string& key, const string& value) {
    try {
        return stoi(value);
    } catch (...) {
        printError(format("Невірне число для {}: '{}'. Введіть ціле число (напр. 41)", key, value));
        return nullopt;
    }
}

optional<string> CLI::getStringArg(const map<string, string>& args, const string& key, const string& altKey,
                                   bool required) {
    if (!checkArg(args, key, altKey, required)) {
        return nullopt;
    }
    if (args.contains(key)) {
        return args.at(key);
    }
    return args.at(altKey);
}

optional<string> CLI::getRequiredStringArg(const map<string, string>& args, const string& key, const string& altKey) {
    return getStringArg(args, key, altKey, true);
}

optional<string> CLI::getOptionalStringArg(const map<string, string>& args, const string& key, const string& altKey) {
    return getStringArg(args, key, altKey, false);
}

bool CLI::checkArg(const map<string, string>& args, const string& key, const string& altKey, bool required) {
    if (!args.contains(key) && !args.contains(altKey)) {
        if (required) printError(format("Додайте параметр {} або {}", key, altKey));
        return false;
    }
    if (args.contains(key) && args.contains(altKey)) {
        printError(format("Використовуйте один параметр {} або {}", key, altKey));
        return false;
    }
    return true;
}

void CLI::showComponents(const vector<const Component*>& components) {
    printDivider();
    for (auto* comp : components) {
        printComponent(comp);
        printDivider();
    }
}

void CLI::showComponent(const Component* comp) {
    printDivider();
    printComponent(comp);
    printDivider();
}

void CLI::printComponent(const Component* component) {
    cout << CLI_COMPONENT_CLR << component->toString() << CLI_RESET_CLR;
}

void CLI::printDivider() {
    cout << "----------------------------------------" << endl;
}

bool CLI::checkAccess() const {
    if (!currentUser->isAdmin()) {
        printError("Для цієї команди потрібні права адміністратора.");
        return false;
    }
    return true;
}

bool CLI::confirm(const string& prompt) {
    printInfo(prompt + " [y/n]: ");
    string ans;
    getline(cin, ans);
    return ans == "y" || ans == "Y";
}

void CLI::printPrompt() const {
    const string role = currentUser->isAdmin() ? string("admin") : string("guest");
    cout << CLI_INF_CLR << "[" << role << "] > " << CLI_RESET_CLR;
}

void CLI::printError(const string& msg) {
    cerr << CLI_ERR_COLOR << msg << CLI_RESET_CLR << "\n";
}

void CLI::printInfo(const string& msg) {
    cout << CLI_INF_CLR << msg << CLI_RESET_CLR << "\n";
}

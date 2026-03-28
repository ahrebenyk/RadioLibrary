#include "services/UserInterface.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <format>
#include <thread>

#include "components/Resistor.h"
#include "components/Transistor.h"
#include "services/JsonFileService.h"

UserInterface::UserInterface(DataService& dataService, shared_ptr<User> user) :
    ds(dataService), currentUser(user) {
}

void UserInterface::start() {
    clearScreen();
    printMenuItem("---------- Оберіть користувача ---------");
    printMenuItem("1. Гість (лише перегляд)");
    printMenuItem("2. Адміністратор (повний доступ)");
    printMenuItem("0. Вихід");
    printMenuLine();
    char option = getSelectedOption({'1', '2', '0'});
    switch (option) {
    case '1': {
        currentUser->setRole(UserRole::GUEST);
        break;
    }
    case '2': {
        currentUser->setRole(UserRole::ADMIN);
        break;
    }
    case '0':
        return;
    default:
        showError("Невірний пункт меню, натисніть цифру для вибору");
    }
    showMainMenu();
}

void UserInterface::showMainMenu() {
    while (true) {
        clearScreen();
        printMenuItem("------------- ГОЛОВНЕ МЕНЮ -------------");
        printMenuItem("1. Показати всі компоненти");
        printMenuItem("2. Пошук за типом");
        printMenuItem("3. Пошук за ID");
        printMenuItem("4. Пошук за назвою");
        if (currentUser->isAdmin()) {
            printMenuItem("5. Додати компонент");
            printMenuItem("6. Редагувати компонент");
            printMenuItem("7. Видалити компонент");
        }
        printMenuItem("0. Вихід");
        printMenuLine();

        vector<char> allowedOptions = currentUser->isAdmin() ? ADMIN_OPTIONS : GUEST_OPTIONS;

        switch (getSelectedOption(allowedOptions)) {
        case '1':
            clearScreen();
            printAllComponents();
            awaitKey();
            break;
        case '2':
            searchByTypeMenu();
            break;
        case '3':
            getByIdMenu();
            break;
        case '4':
            searchByNameMenu();
            break;
        case '5':
            addComponentMenu();
            break;
        case '6':
            editComponentMenu();
            break;
        case '7':
            deleteByIdMenu();
            break;
        case '0':
            return;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }
}

void UserInterface::searchByTypeMenu() {
    clearScreen();
    auto componentType = readComponentType();
    clearScreen();
    printComponentsByType(componentType);
    awaitKey();
}

void UserInterface::getByIdMenu() {
    clearScreen();
    int searchId = readInt("Введіть ID компонента:");;
    clearScreen();
    Component* result = ds.getById(searchId);

    if (result) {
        printInfoItem("\nЗнайдено компонент:");
        printMenuLine();
        result->showInfo();
        printMenuLine();
    } else {
        printErrorItem(format("\nКомпонент з ID {} не знайдено", searchId));
    }
    awaitKey();
}

void UserInterface::deleteByIdMenu() {
    clearScreen();
    int id = readInt("Введіть ID для видалення:");
    clearScreen();
    Component* result = ds.getById(id);

    if (result) {
        printInfoItem("Знайдено компонент:");
        printMenuLine();
        result->showInfo();
        printMenuLine();
        bool confirm = readConfirm("Ви впевнені що хочете видалити цей компонент?");
        if (confirm) {
            if (ds.deleteById(id)) {
                printInfoItem("Компонент видалено");
            }
        } else {
            printInfoItem("Видалення скасовано");;
        }
    } else {
        printErrorItem(format("\nКомпонент з ID {} не знайдено", id));
    }
    awaitKey();
}

void UserInterface::searchByNameMenu() {
    clearScreen();
    string namePart;
    printInfoItem("Введіть частину назви компонента:");
    getline(cin >> std::ws, namePart);
    vector<Component*> results = ds.searchByName(namePart);
    printInfoItem(format("\nЗнайдено компонентів за назвою '{}' : {}", namePart, results.size()));
    printComponents(results);
    awaitKey();
}

void UserInterface::addComponentMenu() {
    clearScreen();
    printMenuItem("------ Додавання нового компонента -----");
    printLine("");

    auto compType = readComponentType();
    clearScreen();
    printMenuItem("------ Додавання нового компонента -----");
    printMenuItem("Тип компонента: ");
    printLine(componentTypeToUkString(compType));

    auto compName = readString("Введіть назву");

    int id = ds.getNextId();
    unique_ptr<Component> newComp = nullptr;

    if (compType == ComponentType::Resistor) {
        double res = readDouble("Введіть опір (Ом): ");
        double pwr = readDouble("Введіть потужність (Вт): ");
        newComp = make_unique<Resistor>(id, compName, res, pwr);
    } else if (compType == ComponentType::Diode) {
        double volt = readDouble("Введіть напругу (В): ");
        double curr = readDouble("Введіть струм (А): ");
        string material = readString("Введіть матеріал: ");
        newComp = make_unique<Diode>(id, compName, curr, volt, material);
    } else if (compType == ComponentType::Transistor) {
        string polarity = readString("Введіть полярність (NPN/PNP): ");
        double volt = readDouble("Введіть напругу (В): ");
        double curr = readDouble("Введіть струм (А): ");
        double gain = readDouble("Введіть підсилення: ");
        newComp = make_unique<Transistor>(id, compName, polarity, volt, curr, gain);
    } else {
        double volt = readDouble("Введіть напругу (В): ");
        double capacity = readDouble("Введіть ємність (мФ): ");
        newComp = make_unique<Capacitor>(id, compName, volt, capacity);
    }

    if (newComp) {
        printMenuLine();
        printInfoItem("Деталі компонента:");
        printMenuLine();
        newComp->showInfo();
        printMenuLine();
        auto confirm = readConfirm("Підтвердіть додавання");
        if (confirm) {
            ds.add(std::move(newComp));
            printMenuItem("Компонент успішно додано");
        } else {
            printMenuItem("Додавання скасовано");
        }
    }
    awaitKey();
}

void UserInterface::editComponentMenu() {
    clearScreen();
    int id = readInt("Введіть ID компонента для редагування:");
    Component* comp = ds.getById(id);

    if (!comp) {
        printErrorItem(format("\nКомпонент з ID {} не знайдено", id));
        awaitKey();
        return;
    }

    if (comp->getType() == ComponentType::Resistor) {
        editResistorMenu(comp);
    } else if (comp->getType() == ComponentType::Diode) {
        editDiodeMenu(comp);
    } else if (comp->getType() == ComponentType::Transistor) {
        editTransistorMenu(comp);
    } else if (comp->getType() == ComponentType::Capacitor) {
        editCapacitorMenu(comp);
    }
}

void UserInterface::editResistorMenu(Component* component) {
    auto* resistor = dynamic_cast<Resistor*>(component);
    clearScreen();

    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("--------- Редагування резистора --------");
        printMenuLine();
        resistor->showInfo();
        printMenuLine();
        printMenuItem("1. Змінити назву");
        printMenuItem("2. Змінити опір");
        printMenuItem("3. Змінити потужність");
        printMenuItem("0. Повернутися");
        printMenuLine();

        switch (getSelectedOption({'1', '2', '3', '0'})) {
        case '1': {
            resistor->setName(readString("Введіть нову назву: "));
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            resistor->setResistance(readDouble("Введіть нове значення опору (Ом): "));
            showInfoMessage("Значення опору змінено");
            break;
        }
        case '3': {
            resistor->setPower(readDouble("Введіть нове значення потужності (Вт): "));
            showInfoMessage("Значення потужності змінено");
            break;
        }
        case '0':
            editing = false;
            break;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }
    ds.save();
}

void UserInterface::editDiodeMenu(Component* component) {
    auto* diode = dynamic_cast<Diode*>(component);
    clearScreen();

    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("----------- Редагування діода ----------");
        printMenuLine();
        diode->showInfo();
        printMenuLine();
        printMenuItem("1. Змінити назву");
        printMenuItem("2. Змінити струм");
        printMenuItem("3. Змінити напругу");
        printMenuItem("4. Змінити матеріал");
        printMenuItem("0. Повернутися");
        printMenuLine();

        switch (getSelectedOption({'1', '2', '3', '4', '0'})) {
        case '1': {
            diode->setName(readString("Введіть нову назву: "));
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            diode->setCurrent(readDouble("Введіть нове значення струму (А): "));
            showInfoMessage("Значення струму змінено");
            break;
        }
        case '3': {
            diode->setVoltage(readDouble("Введіть нове значення напруги (В): "));
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '4': {
            diode->setMaterial(readString("Введіть нове значення матеріалу: "));
            showInfoMessage("Значення матеріалу змінено");
            break;
        }
        case '0':
            editing = false;
            break;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }
    ds.save();
}

void UserInterface::editTransistorMenu(Component* component) {
    auto* diode = dynamic_cast<Transistor*>(component);
    clearScreen();

    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("-------- Редагування транзистора -------");
        printMenuLine();
        diode->showInfo();
        printMenuLine();
        printMenuItem("1. Змінити назву");
        printMenuItem("2. Змінити провідність");
        printMenuItem("3. Змінити напругу");
        printMenuItem("4. Змінити струм");
        printMenuItem("5. Змінити підсилення");
        printMenuItem("0. Повернутися");
        printMenuLine();

        switch (getSelectedOption({'1', '2', '3', '4', '5', '0'})) {
        case '1': {
            diode->setName(readString("Введіть нову назву: "));
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            diode->setPolarity(readString("Введіть нове значення провідності: "));
            showInfoMessage("Значення провідності змінено");
            break;
        }
        case '3': {
            diode->setVoltage(readDouble("Введіть нове значення напруги (В): "));
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '4': {
            diode->setCurrent(readDouble("Введіть нове значення струму: "));
            showInfoMessage("Значення струму змінено");
            break;
        }
        case '5': {
            diode->setGain(readDouble("Введіть нове значення підсилення: "));
            showInfoMessage("Значення підсилення змінено");
            break;
        }
        case '0':
            editing = false;
            break;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }
    ds.save();
}

void UserInterface::editCapacitorMenu(Component* component) {
    auto* diode = dynamic_cast<Capacitor*>(component);
    clearScreen();
    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("------- Редагування конденсатора -------");
        printMenuLine();
        diode->showInfo();
        printMenuLine();
        printMenuItem("1. Змінити назву");
        printMenuItem("2. Змінити напругу");
        printMenuItem("3. Змінити ємність");
        printMenuItem("0. Повернутися");
        printMenuLine();

        switch (getSelectedOption({'1', '2', '3', '0'})) {
        case '1': {
            diode->setName(readString("Введіть нову назву: "));
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            diode->setVoltage(readDouble("Введіть нове значення напруги (В): "));
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '3': {
            diode->setCapacity(readDouble("Введіть нове значення ємності (мФ): "));
            showInfoMessage("Значення ємності змінено");
            break;
        }
        case '0':
            editing = false;
            break;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }

    ds.save();
}

void UserInterface::printAllComponents() {
    auto results = ds.getAll();
    printInfoItem(format("\nЗнайдено компонентів: {}", results.size()));
    printComponents(results);
}

void UserInterface::printComponentsByType(ComponentType type) {
    auto results = ds.searchByType(type);
    printInfoItem(format("\nЗнайдено компонентів за типом '{}' : {}", componentTypeToUkString(type), results.size()));
    printComponents(results);
}

void UserInterface::printComponents(const std::vector<Component*>& components) {
    printMenuLine();
    for (auto* comp : components) {
        comp->showInfo();
        printMenuLine();
    }
}

char UserInterface::getSelectedOption(const std::vector<char>& allowedChars) {
    printInfoItem("Натисніть відповідну клавішу для вибору");
    do {
        char input = _getch();
        if (std::find(allowedChars.begin(), allowedChars.end(), input) != allowedChars.end()) {
            return input;
        }
        showError("Невірний пункт меню, натисніть цифру для вибору");
    } while (true);
}

int UserInterface::readInt(const string& prompt) {
    int value;
    while (true) {
        printInfoItem(prompt);
        if (cin >> value) {
            clearInput();
            return value;
        }
        printErrorItem("Помилка: невірний формат числа (напр. 10)");
        clearInput();
    }
}

ComponentType UserInterface::readComponentType() {
    string errorMessage;
    while (true) {
        clearScreen();
        printMenuItem("------- Виберіть тип компонента -------");
        printMenuItem("1. Резистор");
        printMenuItem("2. Діод");
        printMenuItem("3. Транзистор");
        printMenuItem("4. Конденсатор");
        printMenuLine();
        switch (getSelectedOption({'1', '2', '3', '4'})) {
        case '1':
            return ComponentType::Resistor;
        case '2':
            return ComponentType::Diode;
        case '3':
            return ComponentType::Transistor;
        case '4':
            return ComponentType::Capacitor;
        default:
            showError("Невірний пункт меню, натисніть цифру для вибору");
        }
    }
}

double UserInterface::readDouble(const string& prompt) {
    string input;
    while (true) {
        printInfoItem(prompt);
        cin >> input;
        replace(input.begin(), input.end(), ',', '.');
        try {
            return stod(input);
        } catch (...) {
            printErrorItem("Помилка: невірний формат числа (напр. 12.1)");
        }
    }
}

string UserInterface::readString(const string& prompt) {
    string input;
    while (true) {
        printInfoItem(prompt);
        if (getline(cin >> std::ws, input)) {
            if (!input.empty() && input.find_first_not_of(" \t\n\v\f\r") != string::npos) {
                return input;
            }
        }
        printErrorItem("Помилка: поле не може бути порожнім!");
        if (cin.fail()) {
            clearInput();
        }
    }
}

bool UserInterface::readConfirm(const string& prompt) {
    char ch;
    while (true) {
        printInfoItem(prompt + " (y/n):");
        ch = _getch();
        char lowerCh = tolower(ch);
        if (lowerCh == 'y') {
            return true;
        }
        if (lowerCh == 'n') {
            return false;
        }
        printMenuItem("Будь ласка, натисніть Y (так) або N (Ні).");
    }
}

void UserInterface::clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void UserInterface::printMenuLine() {
    printMenuItem("----------------------------------------");
}

void UserInterface::printMenuItem(const string& item) {
    cout << MENU_COLOR << item << RESET_COLOR << endl;
}

void UserInterface::printErrorItem(const string& item) {
    cout << ERROR_COLOR << item << RESET_COLOR << endl;
}

void UserInterface::printInfoItem(const string& item) {
    cout << INFO_COLOR << item << RESET_COLOR << endl;
}

void UserInterface::showError(const string& message) {
    cout << ERROR_COLOR;
    showTemporaryMessage(message);
    cout << RESET_COLOR;
}

void UserInterface::showInfoMessage(const string& message) {
    cout << INFO_COLOR;
    showTemporaryMessage(message);
    cout << RESET_COLOR;
}

void UserInterface::showTemporaryMessage(const string& message) {
    cout << message;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout << "\r" << std::string(message.length() + 15, ' ') << "\r" << flush;
}

void UserInterface::printLine(const string& item) {
    cout << item << endl;
}

void UserInterface::clearScreen() {
    system("cls");
}

void UserInterface::awaitKey() {
    printInfoItem("\nНатисніть будь-яку клавішу, щоб повернутися...");
    _getch();
}

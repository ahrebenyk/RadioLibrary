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
    selectUserMenu();
    showMainMenu();
}

void UserInterface::selectUserMenu() {
    clearScreen();
    printMenuItem("---------- Оберіть користувача ---------");
    printMenuItem("1. Гість (лише перегляд)");
    printMenuItem("2. Адміністратор (повний доступ)");
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
    default:
        showError("Невірний пункт меню, натисніть цифру для вибору");
    }
}

void UserInterface::showMainMenu() {
    bool isRunning = true;
    while (isRunning) {
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
        printMenuItem("0. Змінити користувача");
        printMenuItem("Q. Вихід");
        printMenuLine();

        vector<char> allowedOptions = currentUser->isAdmin() ? ADMIN_OPTIONS : GUEST_OPTIONS;

        try {
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
                selectUserMenu();
                break;
            case 27:
            case 'q':
            case 'Q':
                isRunning = false;
                break;
            default:
                showError("Невірний пункт меню, натисніть цифру для вибору");
            }
        } catch (const std::exception& e) {
            printErrorItem(format("Помилка: {}", e.what()));
            awaitKey();
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
    const Component* result = ds.getById(searchId);

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
    const Component* result = ds.getById(id);

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
    vector<const Component*> results = ds.searchByName(namePart);
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
    printInfoItem("Тип компонента: ");
    printLine(componentTypeToUkString(compType));

    if (compType == ComponentType::Resistor) {
        addResistorMenu();
    } else if (compType == ComponentType::Diode) {
        addDiodeMenu();
    } else if (compType == ComponentType::Transistor) {
        addTransistorMenu();
    } else if (compType == ComponentType::Capacitor) {
        addCapacitorMenu();
    }
    awaitKey();
}

void UserInterface::addResistorMenu() {
    auto name = readString("Введіть назву:");
    double res = readDouble("Введіть опір (Ом):");
    double pwr = readDouble("Введіть потужність (Вт):");
    addAfterConfirmation([&] {
        ds.addResistor(name, res, pwr);
    });
}

void UserInterface::addDiodeMenu() {
    auto name = readString("Введіть назву:");
    double volt = readDouble("Введіть напругу (В): ");
    double curr = readDouble("Введіть струм (А): ");
    string material = readString("Введіть матеріал: ");
    addAfterConfirmation([&] {
        ds.addDiode(name, volt, curr, material);
    });
}

void UserInterface::addTransistorMenu() {
    auto name = readString("Введіть назву:");
    string polarity = readString("Введіть полярність: ");
    double volt = readDouble("Введіть напругу (В): ");
    double curr = readDouble("Введіть струм (А): ");
    double gain = readDouble("Введіть підсилення: ");
    addAfterConfirmation([&] {
        ds.addTransistor(name, polarity, volt, curr, gain);
    });
}

void UserInterface::addCapacitorMenu() {
    auto name = readString("Введіть назву:");
    double volt = readDouble("Введіть напругу (В): ");
    double capacity = readDouble("Введіть ємність (мФ): ");
    addAfterConfirmation([&] {
        ds.addCapacitor(name, volt, capacity);
    });
}

void UserInterface::addAfterConfirmation(const function<void()>& action) {
    if (readConfirm("Підтвердіть додавання компонента")) {
        action();
        printMenuItem("Компонент успішно додано");
    } else {
        printMenuItem("Додавання скасовано");
    }
}

void UserInterface::editComponentMenu() {
    clearScreen();
    int id = readInt("Введіть ID компонента для редагування:");
    const Component* comp = ds.getById(id);

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

void UserInterface::editResistorMenu(const Component* component) {
    auto* resistor = dynamic_cast<const Resistor*>(component);
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
            string updatedName = readString("Введіть нову назву: ");
            ds.updateResistor(resistor->getId(), updatedName, nullopt, nullopt);
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            double updatedResistance = readDouble("Введіть нове значення опору (Ом): ");
            ds.updateResistor(resistor->getId(), nullopt, updatedResistance, nullopt);
            showInfoMessage("Значення опору змінено");
            break;
        }
        case '3': {
            double updatedPower = readDouble("Введіть нове значення потужності (Вт): ");
            ds.updateResistor(resistor->getId(), nullopt, nullopt, updatedPower);
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
}

void UserInterface::editDiodeMenu(const Component* component) {
    auto* diode = dynamic_cast<const Diode*>(component);
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
            string newName = readString("Введіть нову назву: ");
            ds.updateDiode(diode->getId(), newName, nullopt, nullopt, nullopt);
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            double newCurrent = readDouble("Введіть нове значення струму (А): ");
            ds.updateDiode(diode->getId(), nullopt, newCurrent, nullopt, nullopt);
            showInfoMessage("Значення струму змінено");
            break;
        }
        case '3': {
            double voltage = readDouble("Введіть нове значення напруги (В): ");
            ds.updateDiode(diode->getId(), nullopt, nullopt, voltage, nullopt);
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '4': {
            string newMaterial = readString("Введіть нове значення матеріалу: ");
            ds.updateDiode(diode->getId(), nullopt, nullopt, nullopt, newMaterial);
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
}

void UserInterface::editTransistorMenu(const Component* component) {
    auto* transistor = dynamic_cast<const Transistor*>(component);
    clearScreen();

    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("-------- Редагування транзистора -------");
        printMenuLine();
        transistor->showInfo();
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
            string newName = readString("Введіть нову назву: ");
            ds.updateTransistor(transistor->getId(), newName, nullopt, nullopt, nullopt, nullopt);
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            string newPolarity = readString("Введіть нове значення провідності: ");
            ds.updateTransistor(transistor->getId(), nullopt, newPolarity, nullopt, nullopt, nullopt);
            showInfoMessage("Значення провідності змінено");
            break;
        }
        case '3': {
            double newVoltage = readDouble("Введіть нове значення напруги (В): ");
            ds.updateTransistor(transistor->getId(), nullopt, nullopt, newVoltage, nullopt, nullopt);
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '4': {
            double newCurrent = readDouble("Введіть нове значення струму: ");
            ds.updateTransistor(transistor->getId(), nullopt, nullopt, nullopt, newCurrent, nullopt);
            showInfoMessage("Значення струму змінено");
            break;
        }
        case '5': {
            double newGain = readDouble("Введіть нове значення підсилення: ");
            ds.updateTransistor(transistor->getId(), nullopt, nullopt, nullopt, nullopt, newGain);
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
}

void UserInterface::editCapacitorMenu(const Component* component) {
    auto* capacitor = dynamic_cast<const Capacitor*>(component);
    clearScreen();
    bool editing = true;
    while (editing) {
        clearScreen();
        printMenuItem("------- Редагування конденсатора -------");
        printMenuLine();
        capacitor->showInfo();
        printMenuLine();
        printMenuItem("1. Змінити назву");
        printMenuItem("2. Змінити напругу");
        printMenuItem("3. Змінити ємність");
        printMenuItem("0. Повернутися");
        printMenuLine();

        switch (getSelectedOption({'1', '2', '3', '0'})) {
        case '1': {
            string newName = readString("Введіть нову назву: ");
            ds.updateCapacitor(capacitor->getId(), newName, nullopt, nullopt);
            showInfoMessage("Назву змінено");
            break;
        }
        case '2': {
            double newVoltage = readDouble("Введіть нове значення напруги (В): ");
            ds.updateCapacitor(capacitor->getId(), nullopt, newVoltage, nullopt);
            showInfoMessage("Значення напруги змінено");
            break;
        }
        case '3': {
            double capacity = readDouble("Введіть нове значення ємності (мФ): ");
            ds.updateCapacitor(capacitor->getId(), nullopt, nullopt, capacity);
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

void UserInterface::printComponents(const vector<const Component*>& components) {
    printMenuLine();
    for (auto* comp : components) {
        comp->showInfo();
        printMenuLine();
    }
}

char UserInterface::getSelectedOption(const vector<char>& allowedChars) {
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

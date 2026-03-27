#include "services/UserInterface.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <format>

#include "components/Resistor.h"
#include "components/Transistor.h"
#include "services/JsonFileService.h"

UserInterface::UserInterface(DataService& dataService) : ds(dataService) {
}

void UserInterface::showMainMenu() {
    while (true) {
        clearScreen();
        printMenuItem("------------- ГОЛОВНЕ МЕНЮ -------------");
        printMenuItem("1. Показати всі компоненти");
        printMenuItem("2. Пошук за типом");
        printMenuItem("3. Пошук за ID");
        printMenuItem("4. Пошук за назвою");
        printMenuItem("5. Додати компонент");
        printMenuItem("6. Видалити компонент");
        printMenuItem("0. Вихід");
        printMenuLine();
        printMenuItem("Натисніть відповідну клавішу для вибору");

        char choice = _getch();

        switch (choice) {
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
            deleteByIdMenu();
            break;
        case '0':
            return;
        default:
            printMenuItem("Невірний пункт меню");
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
    Component* result = ds.getById(searchId);

    if (result) {
        printMenuItem("\nЗнайдено компонент:");
        printMenuLine();
        result->showInfo();
        printMenuLine();
    } else {
        printMenuItem(format("\nКомпонент з ID {} не знайдено", searchId));
    }
    awaitKey();
}

void UserInterface::deleteByIdMenu() {
    clearScreen();
    int id = readInt("Введіть ID для видалення:");
    clearScreen();
    Component* result = ds.getById(id);

    if (result) {
        printMenuItem("Знайдено компонент:");
        printMenuLine();
        result->showInfo();
        printMenuLine();
        bool confirm = readBool("Ви впевнені що хочете видалити цей компонент?");
        if (confirm) {
            if (ds.deleteById(id)) {
                printMenuItem("Компонент видалено");
            }
        } else {
            printMenuItem("Видалення скасовано");;
        }
    } else {
        printMenuItem(format("\nКомпонент з ID {} не знайдено", id));
    }
    awaitKey();
}

void UserInterface::searchByNameMenu() {
    clearScreen();
    string namePart;
    printMenuItem("Введіть частину назви компонента:");
    getline(cin >> std::ws, namePart);
    vector<Component*> results = ds.searchByName(namePart);
    printComponents(results);
    awaitKey();
}

void UserInterface::addComponentMenu() {
    clearScreen();
    printMenuItem("------ ДОДАВАННЯ НОВОГО КОМПОНЕНТУ -----");

    auto compType = readComponentType();
    clearScreen();
    printMenuItem("------ ДОДАВАННЯ НОВОГО КОМПОНЕНТУ -----");
    printMenuItem("Тип компонента: ");
    printLine(componentTypeToUkString(compType));

    auto compName = readString("Введіть назву");

    int id = ds.getNextId();
    unique_ptr<Component> newComp = nullptr;

    if (compType == ComponentType::Resistor) {
        double res = readDouble("Опір (Ом): ");
        double pwr = readDouble("Потужність (Вт): ");
        newComp = make_unique<Resistor>(id, compName, res, pwr);
    } else if (compType == ComponentType::Diode) {
        double curr = readDouble("Струм (А): ");
        double volt = readDouble("Напруга (В): ");
        string material = readString("Матеріал: ");
        newComp = make_unique<Diode>(id, compName, curr, volt, material);
    } else if (compType == ComponentType::Transistor) {
        string polarity = readString("Полярність (NPN/PNP): ");
        double volt = readDouble("Напруга (В): ");
        double curr = readDouble("Струм (А): ");
        double gain = readDouble("Підсилення: ");
        newComp = make_unique<Transistor>(id, compName, polarity, volt, curr, gain);
    } else {
        double volt = readDouble("Напруга (В): ");
        double capacity = readDouble("Ємність (мФ): ");
        newComp = make_unique<Capacitor>(id, compName, volt, capacity);
    }

    if (newComp) {
        printMenuLine();
        printMenuItem("Деталі компонента:");
        printMenuLine();
        newComp->showInfo();
        printMenuLine();
        auto confirm = readBool("Підтвердіть додавання");
        if (confirm) {
            ds.add(std::move(newComp));
            printMenuItem("Компонент успішно додано");
        } else {
            printMenuItem("Додавання скасовано");
        }
    }
    awaitKey();
}

void UserInterface::printAllComponents() {
    auto results = ds.getAll();
    printComponents(results);
}

void UserInterface::printComponentsByType(ComponentType type) {
    auto results = ds.searchByType(type);
    printComponents(results);
}

void UserInterface::printComponents(const std::vector<Component*>& components) {
    printMenuItem(format("\nЗнайдено компонентів: {} \n", components.size()));
    printMenuLine();
    for (auto* comp : components) {
        comp->showInfo();
        printMenuLine();
    }
}

int UserInterface::readInt(const string& prompt) {
    int value;
    while (true) {
        printMenuItem(prompt);
        if (cin >> value) {
            clearInput();
            return value;
        }
        printMenuItem("Помилка: невірний формат числа (напр. 10)");
        clearInput();
    }
}

ComponentType UserInterface::readComponentType() {
    printMenuItem("------- ВИБЕРІТЬ ТИП КОМПОНЕНТА -------");
    printMenuItem("1. Резистор");
    printMenuItem("2. Діод");
    printMenuItem("3. Транзистор");
    printMenuItem("4. Конденсатор");
    printMenuLine();
    printMenuItem("Натисніть відповідну клавішу для вибору");

    while (true) {
        char choice = _getch();
        switch (choice) {
        case '1':
            return ComponentType::Resistor;
        case '2':
            return ComponentType::Diode;
        case '3':
            return ComponentType::Transistor;
        case '4':
            return ComponentType::Capacitor;
        default:
            printMenuItem("Невірний тип, натисніть 1, 2, 3 або 4 для вибору типу");
        }
    }
}

double UserInterface::readDouble(const string& prompt) {
    string input;
    while (true) {
        printMenuItem(prompt);
        cin >> input;
        replace(input.begin(), input.end(), ',', '.');
        try {
            return stod(input);
        } catch (...) {
            printMenuItem("Помилка: невірний формат числа (напр. 12.1)");
        }
    }
}

string UserInterface::readString(const string& prompt) {
    string input;
    while (true) {
        printMenuItem(prompt);
        if (getline(cin >> std::ws, input)) {
            if (!input.empty() && input.find_first_not_of(" \t\n\v\f\r") != string::npos) {
                return input;
            }
        }
        printMenuItem("Помилка: поле не може бути порожнім!");
        if (cin.fail()) {
            clearInput();
        }
    }
}

bool UserInterface::readBool(const std::string& prompt) {
    char ch;
    while (true) {
        printMenuItem(prompt + " (y/n):");
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
    cout << MENU_COLOR << item << RESET << endl;
}

void UserInterface::printLine(const string& item) {
    cout << item << endl;
}

void UserInterface::clearScreen() {
    system("cls");
}

void UserInterface::awaitKey() {
    printMenuItem("\nНатисніть будь-яку клавішу, щоб повернутися...");
    _getch();
}

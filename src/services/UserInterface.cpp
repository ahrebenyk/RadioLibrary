#include "services/UserInterface.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <format>

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
        printMenuItem("0. Вихід");
        printMenuLine();

        char choice = _getch();

        switch (choice) {
            case '1':
                clearScreen();
                printAllComponents();
                awaitKey();
                break;
            case '2':
                showSearchByTypeMenu();
                break;
            case '3':
                getByIdMenu();
                break;
            case '4':
                searchByNameMenu();
                break;
            case '0':
                return;
            default:
                printMenuItem("Невірний пункт меню");
                awaitKey();
        }

    }
}

void UserInterface::showSearchByTypeMenu() {
    while (true) {
        clearScreen();
        printMenuItem("------- ВИБЕРІТЬ ТИП КОМПОНЕНТІВ -------");
        printMenuItem("1. Резистори");
        printMenuItem("2. Діоди");
        printMenuItem("3. Транзистори");
        printMenuItem("4. Конденсатори");
        printMenuItem("0. Назад у головне меню");
        printMenuLine();

        char choice = _getch();

        switch (choice) {
            case '1':
                clearScreen();
                printComponentsByType(ComponentType::Resistor);
                awaitKey();
                break;
            case '2':
                clearScreen();
                printComponentsByType(ComponentType::Diode);
                awaitKey();
                break;
            case '3':
                clearScreen();
                printComponentsByType(ComponentType::Transistor);
                awaitKey();
                break;
            case '4':
                clearScreen();
                printComponentsByType(ComponentType::Capacitor);
                awaitKey();
                break;
            case '0':
                return;
            default:
                printMenuItem("Невірний пункт меню");
                awaitKey();
        }
    }
}

void UserInterface::getByIdMenu() {
    bool idEntered = false;
    while (!idEntered) {
        clearScreen();
        int searchId;
        printMenuItem("Введіть ID компонента:");

        if (!(cin >> searchId)) {
            printMenuItem("Помилка: введіть ціле число");
            clearInput();
            awaitKey();
            continue;
        }

        idEntered = true;
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

void UserInterface::clearScreen() {
    system("cls");
}

void UserInterface::awaitKey() {
    printMenuItem("\nНатисніть будь-яку клавішу, щоб повернутися...");
    _getch();
}


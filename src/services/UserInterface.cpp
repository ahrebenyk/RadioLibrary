#include "services/UserInterface.h"
#include <iostream>
#include <limits>
#include <conio.h>

UserInterface::UserInterface(DataService& dataService) : ds(dataService) {
}

void UserInterface::clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void UserInterface::showMainMenu() {
    while (true) {
        clearScreen();
        cout << MENU_COLOR << "\n------- ГОЛОВНЕ МЕНЮ -------\n";
        cout << "1. Показати всі компоненти\n";
        cout << "2. Пошук за типом\n";
        cout << "3. Пошук за ID\n";
        cout << "4. Пошук за назвою\n";
        cout << "0. Вихід\n";
        cout << "----------------------------\n";
        cout << endl << RESET;

        char choice = _getch();

        switch (choice) {
            case '1':
                clearScreen();
                ds.print();
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
                cout << "Завершення роботи...\n";
                return;
            default:
                cout << "Невірний пункт меню.\n";
        }

    }
}

void UserInterface::showSearchByTypeMenu() {
    while (true) {
        clearScreen();
        cout << MENU_COLOR << "\n--- ВИБЕРІТЬ ТИП КОМПОНЕНТІВ ---\n";
        cout << "1. Резистори\n";
        cout << "2. Діоди\n";
        cout << "3. Транзистори\n";
        cout << "4. Конденсатори\n";
        cout << "0. Назад у головне меню\n";
        cout << "--------------------------------\n";
        cout << endl << RESET;

        char choice = _getch();

        switch (choice) {
            case '1':
                ds.printByType(ComponentType::Resistor);
                awaitKey();
                break;
            case '2':
                ds.printByType(ComponentType::Diode);
                awaitKey();
                break;
            case '3':
                ds.printByType(ComponentType::Transistor);
                awaitKey();
                break;
            case '4':
                ds.printByType(ComponentType::Capacitor);
                awaitKey();
                break;
            case '0':
                return;
            default: cout << "Невірний тип.\n";
        }
    }
}

void UserInterface::getByIdMenu() {
    clearScreen();
    int searchId;
    cout << MENU_COLOR << "Введіть ID компонента: " << RESET;

    if (!(cin >> searchId)) {
        clearInput();
        return;
    }

    Component* found = ds.getById(searchId);

    if (found) {
        cout << "\nЗнайдено компонент:\n";
        found->showInfo();
    } else {
        cout << "\nКомпонент з ID " << searchId << " не знайдено.\n";
    }
    awaitKey();
}

void UserInterface::searchByNameMenu() {
    clearScreen();
    string namePart;
    cout << MENU_COLOR << "Введіть частину назви компонента: " << RESET;

    getline(cin >> std::ws, namePart);
    vector<Component*> results = ds.searchByName(namePart);

    if (!results.empty()) {
        cout << "\nЗнайдено компонентів: \n" << results.size();
        for (auto* comp : results) {
            comp->showInfo();
        }
    } else {
        cout << "\nКомпонент за ім'ям " << namePart << " не знайдено.\n";
    }
    awaitKey();
}

void UserInterface::clearScreen() {
    system("cls");
}

void UserInterface::awaitKey() {
    cout << MENU_COLOR;
    cout << "\nНатисніть будь-яку клавішу, щоб повернутися...";
    cout << RESET;
    _getch();
}


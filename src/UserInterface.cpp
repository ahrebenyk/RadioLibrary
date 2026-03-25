#include "UserInterface.h"
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
        cout << "\n======= ГОЛОВНЕ МЕНЮ =======\n";
        cout << "1. Показати всі компоненти\n";
        cout << "2. Пошук за типом\n";
        cout << "3. Пошук за ID\n";
        cout << "0. Вихід\n";
        cout << endl;

        char choice = _getch();

        switch (choice) {
            case '1':
                clearScreen();
                ds.print();
                awaitKey();
                break;
            case '2':
                showSubMenu();
                break;
            case '3':
                getByIdMenu();
                break;
            case '0':
                cout << "Завершення роботи...\n";
                return;
            default:
                cout << "Невірний пункт меню.\n";
        }

    }
}

void UserInterface::showSubMenu() {
    while (true) {
        clearScreen();
        cout << "\n--- ВИБЕРІТЬ ТИП КОМПОНЕНТІВ ---\n";
        cout << "1. Резистори\n";
        cout << "2. Діоди\n";
        cout << "3. Транзистори\n";
        cout << "4. Конденсатори\n";
        cout << "0. Назад у головне меню\n";
        cout << endl;

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
    cout << "Введіть ID компонента: ";
    if (!(cin >> searchId)) {
        clearInput();
        return;
    }

    Component* found = ds.getById(searchId);

    if (found) {
        cout << "\nЗнайдено компонент:\n";
        found->showInfo();
    } else {
        cout << "\nПомилка: Компонент з ID " << searchId << " не існує.\n";
    }
    awaitKey();
}

void UserInterface::clearScreen() {
    system("cls");
}

void UserInterface::awaitKey() {
    cout << "\nНатисніть будь-яку клавішу, щоб повернутися...";
    _getch();
}


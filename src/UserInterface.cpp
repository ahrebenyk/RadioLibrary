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
        cout << "\n===== ГОЛОВНЕ МЕНЮ =====\n";
        cout << "1. Показати всі компоненти\n";
        cout << "2. Пошук за типом\n";
        cout << "0. Вихід\n";
        cout << endl;

        char choice = _getch();

        switch (choice) {
            case '1':
                ds.print();
                break;
            case '2':
                showSubMenu();
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
        cout << "\n--- ВИБЕРІТЬ ТИП КОМПОНЕНТУ ---\n";
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
                break;
            case '2':
                ds.printByType(ComponentType::Diode);
                break;
            case '3':
                ds.printByType(ComponentType::Transistor);
                break;
            case '4':
                ds.printByType(ComponentType::Capacitor);
                break;
            case '0':
                return;
            default: cout << "Невірний тип.\n";
        }
    }
}

#include "Resistor.h"
#include <iostream>

using namespace std;

Resistor::Resistor(int id, string name, double res, double pwr)
    : Component(id, name), resistance(res), power(pwr) {}

void Resistor::showInfo() {
    cout << "ID: " << id << "\n";
    cout << "Тип: Резистор\n";
    cout << "Назва: " << name << "\n";
    cout << "Опір: " << resistance << " Ом\n";
    cout << "Потужність: " << power << " Вт\n";
}
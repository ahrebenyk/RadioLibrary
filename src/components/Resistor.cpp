#include "components/Resistor.h"
#include <iostream>

using namespace std;

Resistor::Resistor(int id, string name, double res, double pwr)
    : Component(id, name, ComponentType::Resistor), resistance(res), power(pwr) {}

void Resistor::showInfo() {
    cout << COMPONENT_COLOR;
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tРезистор\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Опір:\t\t" << resistance << " Ом\n";
    cout << "Потужність:\t" << power << " Вт\n";
    cout << RESET_COMPONENT_COLOR;
}
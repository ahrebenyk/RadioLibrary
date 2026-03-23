#include "Diode.h"
#include <iostream>

using namespace std;

Diode::Diode(int id, std::string name, double current, double voltage, std::string mat)
    : Component(id, name), current(current), voltage(voltage), material(mat) {}

void Diode::showInfo() {
    cout << "ID: " << id << "\n";
    cout << "Тип: Діод\n";
    cout << "Назва: " << name << "\n";
    cout << "Струм: " << current << " А\n";
    cout << "Напруга: " << voltage << " В\n";
    cout << "Матеріал: " << material << "\n";
}
#include "components/Diode.h"
#include <iostream>

using namespace std;

Diode::Diode(int id, std::string name, double current, double voltage, string mat)
    : Component(id, name, ComponentType::Diode), current(current), voltage(voltage), material(mat) {}

void Diode::showInfo() {
    cout << COMPONENT_COLOR;
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tДіод\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Струм:\t\t" << current << " А\n";
    cout << "Напруга:\t" << voltage << " В\n";
    cout << "Матеріал:\t" << material << "\n";
    cout << RESET_COMPONENT_COLOR;
}
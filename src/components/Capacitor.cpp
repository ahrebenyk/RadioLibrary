#include "components/Capacitor.h"

#include <iostream>

using namespace std;

Capacitor::Capacitor(int id, string name, double volt, double cap)
    : Component(id, name, ComponentType::Capacitor), voltage(volt), capacity(cap) {}

void Capacitor::showInfo() {
    cout << COMPONENT_COLOR;
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tКонденсатор\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Напруга:\t" << voltage << " В\n";
    cout << "Ємність:\t" << capacity << " мФ\n";
    cout << RESET_COMPONENT_COLOR;
}
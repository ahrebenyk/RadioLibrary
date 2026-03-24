#include "Capacitor.h"

#include <iostream>

using namespace std;

Capacitor::Capacitor(int id, string name, double volt, double cap)
    : Component(id, name, ComponentType::Capacitor), voltage(volt), capacity(cap) {}

void Capacitor::showInfo() {
    cout << "ID: " << id << "\n";
    cout << "Тип: Конденсатор\n";
    cout << "Назва: " << name << "\n";
    cout << "Напруга: " << voltage << " В\n";
    cout << "Ємність: " << capacity << " мФ\n";
}
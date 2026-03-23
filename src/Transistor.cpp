#include "Transistor.h"
#include <iostream>

using namespace std;

Transistor::Transistor(int id, string name, TransistorType type, double voltage, double current, int gain)
    : Component(id, name), type(type), voltage(voltage), current(current), gain(gain) {}

string Transistor::getTypeString() const {
    switch (type) {
    case TransistorType::NPN:       return "NPN";
    case TransistorType::PNP:       return "PNP";
    default:                        return "Unknown";
    }
}

void Transistor::showInfo() {
    cout << "ID: " << id << "\n";
    cout << "Тип: Транзистор\n";
    cout << "Назва: " << name << "\n";
    cout << "Тип провідності: " << getTypeString() << "\n";
    cout << "Напруга: " << voltage << " В\n";
    cout << "Струм: " << current << " А\n";
    cout << "Підсилення: " << gain << "\n";
}
#include "components/Transistor.h"
#include <iostream>

using namespace std;

Transistor::Transistor(int id, string name, TransistorType transistorType, double voltage, double current, int gain)
    : Component(id, name, ComponentType::Transistor), transistorType(transistorType), voltage(voltage), current(current), gain(gain) {}

string Transistor::getTransistorTypeString() const {
    switch (transistorType) {
    case TransistorType::NPN:       return "NPN";
    case TransistorType::PNP:       return "PNP";
    default:                        return "Unknown";
    }
}

void Transistor::showInfo() {
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tТранзистор\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Провідність:\t" << getTransistorTypeString() << "\n";
    cout << "Напруга:\t" << voltage << " В\n";
    cout << "Струм:\t\t" << current << " А\n";
    cout << "Підсилення:\t" << gain << "\n";
}
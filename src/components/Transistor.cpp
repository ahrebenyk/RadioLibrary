#include "components/Transistor.h"
#include <iostream>

using namespace std;

Transistor::Transistor(int id, string name, string polarity, double voltage, double current, int gain)
    : Component(id, name, ComponentType::Transistor), polarity(polarity), voltage(voltage), current(current), gain(gain) {}

void Transistor::showInfo() {
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tТранзистор\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Провідність:\t" << polarity << "\n";
    cout << "Напруга:\t" << voltage << " В\n";
    cout << "Струм:\t\t" << current << " А\n";
    cout << "Підсилення:\t" << gain << "\n";
}
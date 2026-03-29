#include "components/Transistor.h"
#include <iostream>

using namespace std;

Transistor::Transistor(int id, const string& name, const string& polarity, double voltage, double current, double gain)
    : Component(id, name, ComponentType::Transistor), polarity(polarity), voltage(voltage), current(current), gain(gain) {}

void Transistor::showInfo() const {
    cout << COMPONENT_COLOR;
    cout << "ID:\t\t" << id << "\n";
    cout << "Тип:\t\tТранзистор\n";
    cout << "Назва:\t\t" << name << "\n";
    cout << "Провідність:\t" << polarity << "\n";
    cout << "Напруга:\t" << voltage << " В\n";
    cout << "Струм:\t\t" << current << " А\n";
    cout << "Підсилення:\t" << gain << "\n";
    cout << RESET_COMPONENT_COLOR;
}
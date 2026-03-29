#ifndef RADIOLIBRARY_CAPACITOR_H
#define RADIOLIBRARY_CAPACITOR_H

#include "Component.h"

using namespace std;

class Capacitor : public Component {
private:
    double voltage;
    double capacity;
public:
    Capacitor(int id, const string& name, double volt, double cap);
    double getVoltage() const { return voltage; }
    void setVoltage(double volt) { this->voltage = volt; }
    double getCapacity() const { return capacity; }
    void setCapacity(double cap) { this->capacity = cap; }
    void showInfo() const override;
};

#endif
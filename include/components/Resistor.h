#ifndef RADIOLIBRARY_RESISTOR_H
#define RADIOLIBRARY_RESISTOR_H

#include "Component.h"

using namespace std;

class Resistor : public Component {
private:
    double resistance;
    double power;
public:
    Resistor(int id, string name, double res, double pwr);
    double getResistance() const { return resistance; }
    double getPower() const { return power; }
    void showInfo() override;
};

#endif
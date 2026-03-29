#ifndef RADIOLIBRARY_RESISTOR_H
#define RADIOLIBRARY_RESISTOR_H

#include "Component.h"

using namespace std;

class Resistor : public Component {
private:
    double resistance;
    double power;
public:
    Resistor(int id, const string& name, double res, double pwr);
    double getResistance() const { return resistance; }
    void setResistance(double res) { this->resistance = res; }
    double getPower() const { return power; }
    void setPower(double pow) { this->power = pow; }
    void showInfo() const override;
};

#endif
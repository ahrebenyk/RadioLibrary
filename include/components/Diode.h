#ifndef DIODE_H
#define DIODE_H

#include "Component.h"
using namespace std;

class Diode : public Component {
private:
    double current;
    double voltage;
    string material;
public:
    Diode(int id, string name, double current, double voltage, string mat);
    double getCurrent() const { return current; }
    double getVoltage() const { return voltage; }
    string getMaterial() const { return material; }
    void showInfo() override;
};

#endif
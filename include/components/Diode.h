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
    Diode(int id, const string& name, double current, double voltage, const string& mat);
    double getCurrent() const { return current; }
    void setCurrent(double curr) { this->current = curr; }
    double getVoltage() const { return voltage; }
    void setVoltage(double volt) { this->voltage = volt; }
    string getMaterial() const { return material; }
    void setMaterial(const string& mat) { this->material = mat; }
    void showInfo() const override;
};

#endif
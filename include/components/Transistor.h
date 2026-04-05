#ifndef RADIOLIBRARY_TRANSISTOR_H
#define RADIOLIBRARY_TRANSISTOR_H

#include "Component.h"

using namespace std;

class Transistor : public Component {
private:
    string polarity;
    double voltage;
    double current;
    double gain;

public:
    Transistor(int id, const string& name, const string& polarity, double voltage, double current, double gain);
    string toString() const override;
    string getPolarity() const { return polarity; }
    void setPolarity(const string& pol) { this->polarity = pol;}
    double getVoltage() const { return voltage; }
    void setVoltage(double volt) { this->voltage = volt; }
    double getCurrent() const { return current; }
    void setCurrent(double curr) { this->current = curr; }
    double getGain() const { return gain; }
    void setGain(double g) { this->gain = g; }
};
#endif
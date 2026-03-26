#ifndef RADIOLIBRARY_TRANSISTOR_H
#define RADIOLIBRARY_TRANSISTOR_H

#include "Component.h"

using namespace std;

class Transistor : public Component {
private:
    string polarity;
    double voltage;
    double current;
    int gain;

public:
    Transistor(int id, string name, string polarity, double voltage, double current, int gain);
    void showInfo() override;
    string getPolarity() const { return polarity; }
    double getVoltage() const { return voltage; }
    double getCurrent() const { return current; }
    int getGain() const { return gain; }
};
#endif
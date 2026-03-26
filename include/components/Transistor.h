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
};
#endif
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
    void showInfo() override;
};

#endif
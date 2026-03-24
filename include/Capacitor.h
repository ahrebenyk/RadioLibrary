#ifndef RADIOLIBRARY_CAPACITOR_H
#define RADIOLIBRARY_CAPACITOR_H

#include "Component.h"

using namespace std;

class Capacitor : public Component {
private:
    double voltage;
    double capacity;
public:
    Capacitor(int id, string name, double volt, double cap);
    void showInfo() override;
};

#endif
#ifndef RADIOLIBRARY_TRANSISTOR_H
#define RADIOLIBRARY_TRANSISTOR_H

#include "Component.h"

using namespace std;

enum class TransistorType { NPN, PNP };

class Transistor : public Component {
private:
    TransistorType transistorType;
    double voltage;
    double current;
    int gain;

public:
    Transistor(int id, string name, TransistorType transistorType, double voltage, double current, int gain);
    string getTransistorTypeString() const;
    void showInfo() override;
};
#endif
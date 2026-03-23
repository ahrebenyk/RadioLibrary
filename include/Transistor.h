#ifndef RADIOLIBRARY_TRANSISTOR_H
#define RADIOLIBRARY_TRANSISTOR_H

#include "Component.h"

using namespace std;

enum class TransistorType { NPN, PNP, N_CHANNEL, P_CHANNEL };

class Transistor : public Component {
private:
    TransistorType type;
    double voltage;
    double current;
    int gain;

public:
    Transistor(int id, string name, TransistorType type, double voltage, double current, int gain);
    string getTypeString() const;
    void showInfo() override;
};
#endif
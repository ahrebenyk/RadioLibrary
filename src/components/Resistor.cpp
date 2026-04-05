#include "components/Resistor.h"

#include <format>

using namespace std;

Resistor::Resistor(int id, const string& name, double res, double pwr)
    : Component(id, name, ComponentType::Resistor), resistance(res), power(pwr) {
}

string Resistor::toString() const {
    return format(
        "ID:\t\t{}\nТип:\t\t{}\nНазва:\t\t{}\nОпір:\t\t{} Ом\nПотужність:\t{} Вт\n",
        id, componentTypeToUkString(ComponentType::Resistor), name, formatDouble(resistance), formatDouble(power)
    );
}

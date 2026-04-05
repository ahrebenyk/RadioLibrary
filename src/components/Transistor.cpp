#include "components/Transistor.h"

#include <format>

using namespace std;

Transistor::Transistor(int id, const string& name, const string& polarity, double voltage, double current, double gain)
    : Component(id, name, ComponentType::Transistor), polarity(polarity), voltage(voltage), current(current),
      gain(gain) {
}

string Transistor::toString() const {
    return format(
        "Id:\t\t{}\nТип:\t\t{}\nНазва:\t\t{}\nПровідність:\t{}\nНапруга:\t{} В\nСтрум:\t\t{} А\nПідсилення:\t{}\n",
        id, componentTypeToUkString(ComponentType::Transistor), name, polarity,
        formatDouble(voltage), formatDouble(current), formatDouble(gain)
    );
}

#include "components/Diode.h"

#include <format>

using namespace std;

Diode::Diode(int id, const string& name, double voltage, double current, const string& mat)
    : Component(id, name, ComponentType::Diode), current(current), voltage(voltage), material(mat) {
}

string Diode::toString() const {
    return format(
        "Id:\t\t{}\nТип:\t\t{}\nНазва:\t\t{}\nСтрум:\t\t{} А\nНапруга:\t{} В\nМатеріал:\t{}\n",
        id, componentTypeToUkString(ComponentType::Diode), name, formatDouble(current), formatDouble(voltage), material
    );
}

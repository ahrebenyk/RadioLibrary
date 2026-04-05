#include "components/Capacitor.h"

#include <format>

using namespace std;

Capacitor::Capacitor(int id, const string& name, double volt, double cap)
    : Component(id, name, ComponentType::Capacitor), voltage(volt), capacity(cap) {
}

string Capacitor::toString() const {
    return format(
        "Id:\t\t{}\nТип:\t\t{}\nНазва:\t\t{}\nНапруга:\t{} В\nЄмність:\t{} мФ\n",
        id, componentTypeToUkString(ComponentType::Capacitor), name, formatDouble(voltage), formatDouble(capacity)
    );
}

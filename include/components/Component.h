#ifndef RADIOLIBRARY_COMPONENT_H
#define RADIOLIBRARY_COMPONENT_H
#include <string>
using namespace std;

enum class ComponentType { Resistor, Diode, Transistor, Capacitor };

class Component {
protected:
    int id;
    string name;
    ComponentType type;
public:
    Component(int id, string name, ComponentType type);
    ComponentType getType() const { return type; };
    string getName() const { return name; };
    int getId() const { return id; };
    virtual ~Component() = default;
    virtual void showInfo() = 0;
};

inline string componentTypeToString(ComponentType type) {
    switch (type) {
    case ComponentType::Resistor:   return "Resistor";
    case ComponentType::Diode:      return "Diode";
    case ComponentType::Transistor: return "Transistor";
    case ComponentType::Capacitor:  return "Capacitor";
    default:                        return "Unknown";
    }
}

inline string componentTypeToUkString(ComponentType type) {
    switch (type) {
    case ComponentType::Resistor:   return "Резистор";
    case ComponentType::Diode:      return "Діод";
    case ComponentType::Transistor: return "Транзистор";
    case ComponentType::Capacitor:  return "Конденсатор";
    default:                        return "Unknown";
    }
}
#endif
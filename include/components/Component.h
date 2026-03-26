#ifndef RADIOLIBRARY_COMPONENT_H
#define RADIOLIBRARY_COMPONENT_H
#include <string>
using namespace std;

enum class ComponentType { Resistor, Diode, Transistor, Capacitor, Unknown };

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

#endif
#ifndef RADIOLIBRARY_COMPONENT_H
#define RADIOLIBRARY_COMPONENT_H

#include <string>
enum class ComponentType { Resistor, Diode, Transistor, Capacitor, Unknown };

class Component {
protected:
    int id;
    std::string name;
public:
    Component(int id, std::string name);
    virtual ~Component() = default;
    virtual void showInfo() = 0;
};

#endif
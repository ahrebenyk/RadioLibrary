#include "components/Component.h"

#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

Component::Component(int id, const string& name, ComponentType type) : id(id), name(name), type(type) {
}

string Component::formatDouble(double value) {
    std::stringstream ss;
    ss << std::setprecision(10) << std::defaultfloat;
    ss << value;
    return ss.str();
}
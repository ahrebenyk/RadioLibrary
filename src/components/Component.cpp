#include "components/Component.h"
#include <iostream>
using namespace std;

Component::Component(int id, const string& name, ComponentType type) : id(id), name(name), type(type) {
}

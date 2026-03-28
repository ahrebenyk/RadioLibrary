#include <iostream>
#include <windows.h>

#include "services/DataService.h"
#include "services/UserInterface.h"

int main()
{
    std::cout << std::setprecision(10) << std::defaultfloat;
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    auto currentUser = std::make_shared<User>(UserRole::GUEST);
    DataService ds("data/components.json", currentUser);
    ds.load();
    UserInterface ui(ds, currentUser);
    ui.start();
    return 0;
}

#include <iostream>
#include <windows.h>

#include "services/DataService.h"
#include "services/UserInterface.h"

int main()
{
    std::cout << std::setprecision(10) << std::defaultfloat;
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    DataService ds("data/components.json");
    ds.load();
    UserInterface ui(ds);
    ui.showMainMenu();
    return 0;
}

#include <iostream>
#include <windows.h>

#include "services/DataService.h"
#include "services/UserInterface.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    DataService ds("data/components.json");
    ds.load();
    UserInterface ui(ds);
    ui.showMainMenu();

    return 0;
}

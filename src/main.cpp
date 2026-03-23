#include <iostream>
#include <windows.h>

#include "DataService.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    DataService ds("data/components.json");
    ds.load();
    ds.print();

    return 0;
}

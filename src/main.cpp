#include <iostream>
#include <windows.h>

#include "services/CLI.h"
#include "services/DataService.h"

int main()
{
    std::cout << std::setprecision(10) << std::defaultfloat;
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    auto user = std::make_shared<User>(UserRole::ADMIN);
    DataService ds("data/components.json", user);
    ds.load();
    CLI cli(ds, user);
    cli.run();
}

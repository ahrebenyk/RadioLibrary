#ifndef RADIOLIBRARY_USERINTERFACE_H
#define RADIOLIBRARY_USERINTERFACE_H

#include "DataService.h"

class UserInterface {
private:
    DataService& ds;
    void showSubMenu();
    void clearInput();

public:
    UserInterface(DataService& dataService);
    void showMainMenu();
};

#endif

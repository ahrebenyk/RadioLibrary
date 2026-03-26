#ifndef RADIOLIBRARY_USERINTERFACE_H
#define RADIOLIBRARY_USERINTERFACE_H

#include "DataService.h"
const string MENU_COLOR = "\033[38;5;111m";
const string RESET = "\033[0m";

class UserInterface {
private:
    DataService& ds;
    void showSearchByTypeMenu();
    void getByIdMenu();
    void searchByNameMenu();
    void printAllComponents();
    void printComponentsByType(ComponentType type);
    void printComponents(const std::vector<Component*>& components);
    static void clearScreen();
    static void awaitKey();
    void clearInput();

public:
    UserInterface(DataService& dataService);
    void showMainMenu();
};

#endif

#ifndef RADIOLIBRARY_USERINTERFACE_H
#define RADIOLIBRARY_USERINTERFACE_H

#include "DataService.h"
const string MENU_COLOR = "\033[34m";
const string ERROR_COLOR = "\033[31m";
const string INFO_COLOR = "\033[90m";
const string RESET_COLOR = "\033[0m";

class UserInterface {
private:
    DataService& ds;
    void searchByTypeMenu();
    void getByIdMenu();
    void deleteByIdMenu();
    void searchByNameMenu();
    void addComponentMenu();
    void editComponentMenu();
    void editResistorMenu(Component* component);
    void editDiodeMenu(Component* component);
    void editTransistorMenu(Component* component);
    void editCapacitorMenu(Component* component);
    void printAllComponents();
    void printComponentsByType(ComponentType type);
    static void printComponents(const vector<Component*>& components);
    static int readInt(const string& prompt);
    static double readDouble(const string& input);
    static string readString(const string& prompt);
    static ComponentType readComponentType();
    static bool readConfirm(const std::string& prompt);
    static void clearScreen();
    static void awaitKey();
    static void clearInput();
    static void printMenuLine();
    static void printMenuItem(const string& item);
    static void printErrorItem(const string& item);
    static void printInfoItem(const string& item);
    void printLine(const string& item);

public:
    UserInterface(DataService& dataService);
    void showMainMenu();
};

#endif

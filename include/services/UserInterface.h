#ifndef RADIOLIBRARY_USERINTERFACE_H
#define RADIOLIBRARY_USERINTERFACE_H

#include "DataService.h"
#include "users/User.h"

const string MENU_COLOR = "\033[32m";
const string ERROR_COLOR = "\033[31m";
const string INFO_COLOR = "\033[90m";
const string RESET_COLOR = "\033[0m";
const string COMPONENT_COLOR = "\033[37m";

class UserInterface {
private:
    inline static const vector<char> ADMIN_OPTIONS = {'1', '2', '3', '4', '5', '6', '7', '8', '0', 'q', 'Q', 27};
    inline static const vector<char> GUEST_OPTIONS = {'1', '2', '3', '4', '5', '0', 'q', 'Q', 27};
    shared_ptr<User> currentUser;
    DataService& ds;
    void searchByTypeMenu();
    void getByIdMenu();
    void deleteByIdMenu();
    void searchByNameMenu();
    void addComponentMenu();
    void addResistorMenu();
    void addDiodeMenu();
    void addTransistorMenu();
    void addCapacitorMenu();
    static void addAfterConfirmation(const std::function<void()>& action);
    void editComponentMenu();
    void editResistorMenu(const Component* component);
    void editDiodeMenu(const Component* component);
    void editTransistorMenu(const Component* component);
    void editCapacitorMenu(const Component* component);
    void printAllComponents();
    void printComponentsByType(ComponentType type);
    static void printComponents(const vector<const Component*>& components);
    static void printComponent(const Component* component);
    static char getSelectedOption(const vector<char>& allowedChars);
    static int readInt(const string& prompt);
    static double readDouble(const string& input);
    static string readString(const string& prompt);
    static ComponentType readComponentType();
    static bool readConfirm(const string& prompt);
    static void clearScreen();
    static void awaitKey();
    static void clearInput();
    static void printMenuLine();
    static void printMenuItem(const string& item);
    static void printErrorItem(const string& item);
    static void printInfoItem(const string& item);
    static void showError(const string& item);
    static void showInfoMessage(const string& message);
    static void showTemporaryMessage(const string& message);
    static void printLine(const string& item);

public:
    UserInterface(DataService& dataService, shared_ptr<User> user);
    void start();
    void selectUserMenu();
    void showMainMenu();
};

#endif

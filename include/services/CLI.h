#ifndef RADIOLIBRARY_CLI_H
#define RADIOLIBRARY_CLI_H

#include <string>
#include <vector>
#include <map>
#include "DataService.h"
#include "users/User.h"

struct CommandData {
    string action;
    map<string, string> args;
};

static const string CLI_ERR_COLOR = "\033[31m";
static const string CLI_INF_CLR = "\033[36m";
static const string CLI_EXAMPLES_CLR = "\033[2m";
static const string CLI_RESET_CLR = "\033[0m";
static const string CLI_COMPONENT_CLR = "\033[37m";

class CLI {
public:
    CLI(DataService& dataService, shared_ptr<User> user);
    void run();

private:
    DataService& ds;
    shared_ptr<User> currentUser;

    static CommandData parseCommand(const string& input);
    void runCommand(const CommandData& cmd);
    void listComponents(const CommandData& cmd) const;
    static optional<ComponentType> strToCompType(const string& typeStr);
    static optional<int> getRequiredIntArg(const map<string, string>& args, const string& key);
    static optional<int> getOptionalIntArg(const map<string, string>& args, const string& key);
    void addComponent(const CommandData& cmd);
    static optional<int> getIntArg(const map<string, string>& args, const string& key, bool required);
    static optional<double> getRequiredDoubleArg(const map<string, string>& args, const string& key, const string& altKey);
    static optional<double> getOptionalDoubleArg(const map<string, string>& args, const string& key, const string& altKey);
    static optional<double> getDoubleArg(const map<string, string>& args, const string& key, const string& altKey, bool required);
    static optional<double> parseDouble(const string& key, const string& value);
    static optional<int> parseInt(const string& key, const string& value);
    static optional<string> getStringArg(const map<string, string>& args, const string& key, const string& altKey, bool required);
    static optional<string> getRequiredStringArg(const map<string, string>& args, const string& key, const string& altKey);
    static optional<string> getOptionalStringArg(const map<string, string>& args, const string& key, const string& altKey);
    static bool checkArg(const map<string, string>& args, const string& key, const string& altKey, bool required);
    void editComponent(const CommandData& cmd);
    void deleteComponent(const CommandData& cmd);
    void switchUser(const CommandData& cmd);
    void showHelp();
    static void showViewCommandsHelp();
    static void showEditCommandsHelp();
    static void printDivider();
    static void showComponents(const vector<const Component*>& list);
    void showComponent(const Component* comp);
    static void printComponent(const Component* comp);
    bool checkAccess() const;
    static bool confirm(const string& prompt);
    void printPrompt() const;
    static void printError(const string& msg);
    static void printInfo (const string& msg);
};

#endif
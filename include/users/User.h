#ifndef RADIOLIBRARY_USER_H
#define RADIOLIBRARY_USER_H

enum class UserRole {
    GUEST,
    ADMIN
};

class User {
private:
    UserRole role;
public:
    User(UserRole userRole = UserRole::GUEST) : role(userRole) {}
    UserRole getRole() const { return role; }
    void setRole(UserRole newRole) { role = newRole; }
    bool isAdmin() const { return role == UserRole::ADMIN; }
    string getRoleString() const {
        return isAdmin() ? "Адміністратор" : "Гість";
    }
};
#endif
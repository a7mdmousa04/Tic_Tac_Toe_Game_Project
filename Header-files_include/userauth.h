// userauth.h - User Authentication System
#ifndef USERAUTH_H
#define USERAUTH_H

#include <QString>
#include <QMap>
#include <QCryptographicHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct User {
    QString username;
    QString passwordHash;
    QJsonArray gameHistory;
};

class UserAuth {
public:
    UserAuth();
    ~UserAuth();

    bool signUp(const QString& username, const QString& password);
    bool signIn(const QString& username, const QString& password);
    bool isLoggedIn() const;
    QString getCurrentUser() const;
    void signOut();
    bool saveGameToHistory(const QJsonObject& gameData);
    QJsonArray getGameHistory() const;

private:
    QMap<QString, User> users;
    QString currentUser;
    bool loggedIn;
    bool isValidEmail(const QString& email);
    bool isValidPassword(const QString& password);
    bool isValidUsername(const QString& username);
    QString hashPassword(const QString& password);
    bool loadUsersFromFile();
    bool saveUsersToFile();
    QString usersFilePath;
};

#endif // USERAUTH_H

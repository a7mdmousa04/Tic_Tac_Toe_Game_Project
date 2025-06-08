// userauth.cpp - Implementation of User Authentication
#include "userauth.h"
#include <QDir>
#include <QStandardPaths>

UserAuth::UserAuth() : loggedIn(false) {
    // Set up file path for user data
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    usersFilePath = dataPath + "/users.json";

    // Load existing users
    loadUsersFromFile();
}

UserAuth::~UserAuth() {
    // Save users when the program exits
    saveUsersToFile();
}

QString UserAuth::hashPassword(const QString& password) {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool UserAuth::signUp(const QString& username, const QString& password) {
    // Check if username already exists
    if (users.contains(username)) {
        return false;
    }

    // Validate username format
    if (!isValidUsername(username)) {
        return false;
    }
    // Validate password
    if (!isValidPassword(password)) {
        return false;
    }

    // Create new user
    User newUser;
    newUser.username = username;
    newUser.passwordHash = hashPassword(password);

    // Add user to map
    users[username] = newUser;

    // Save updated users to file
    return saveUsersToFile();
}
bool UserAuth::signIn(const QString& username, const QString& password) {
    // Check if user exists
    if (!users.contains(username)) {
        return false;
    }

    // Verify password
    if (users[username].passwordHash == hashPassword(password)) {
        currentUser = username;
        loggedIn = true;
        return true;
    }

    return false;
}

bool UserAuth::isLoggedIn() const {
    return loggedIn;
}

QString UserAuth::getCurrentUser() const {
    return currentUser;
}

void UserAuth::signOut() {
    currentUser = "";
    loggedIn = false;
}

bool UserAuth::saveGameToHistory(const QJsonObject& gameData) {
    if (!loggedIn) {
        return false;
    }

    // Add game to user's history
    users[currentUser].gameHistory.append(gameData);

    // Save updated users to file
    return saveUsersToFile();
}

QJsonArray UserAuth::getGameHistory() const {
    if (!loggedIn || !users.contains(currentUser)) {
        return QJsonArray();
    }

    return users[currentUser].gameHistory;
}

bool UserAuth::loadUsersFromFile() {
    QFile file(usersFilePath);
    if (!file.exists()) {
        return true; // No file yet, that's okay
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isObject()) {
        QJsonObject usersObj = doc.object();

        for (auto it = usersObj.begin(); it != usersObj.end(); ++it) {
            QString username = it.key();
            QJsonObject userObj = it.value().toObject();

            User user;
            user.username = username;
            user.passwordHash = userObj["passwordHash"].toString();
            user.gameHistory = userObj["gameHistory"].toArray();

            users[username] = user;
        }
    }

    file.close();
    return true;
}

bool UserAuth::saveUsersToFile() {
    QFile file(usersFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject usersObj;

    for (auto it = users.begin(); it != users.end(); ++it) {
        QJsonObject userObj;
        userObj["passwordHash"] = it.value().passwordHash;
        userObj["gameHistory"] = it.value().gameHistory;

        usersObj[it.key()] = userObj;
    }

    QJsonDocument doc(usersObj);
    file.write(doc.toJson());
    file.close();

    return true;
}

bool UserAuth::isValidPassword(const QString& password) {
    // At least 8 characters
    if (password.length() < 8) {
        return false;
    }

    // At least one uppercase letter
    bool hasUppercase = false;
    for (const QChar& ch : password) {
        if (ch.isUpper()) {
            hasUppercase = true;
            break;
        }
    }
    if (!hasUppercase) {
        return false;
    }

    // At least one special character
    QString specialChars = "!@#$%^&*()_-+=<>?/[]{}|\\";
    bool hasSpecial = false;
    for (const QChar& ch : password) {
        if (specialChars.contains(ch)) {
            hasSpecial = true;
            break;
        }
    }

    return hasSpecial;
}

bool UserAuth::isValidUsername(const QString& username) {
    // Check if username contains only letters, numbers, and underscores
    for (const QChar& ch : username) {
        if (!ch.isLetterOrNumber() && ch != '_') {
            return false;
        }
    }

    return true;
}

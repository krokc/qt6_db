#pragma once
#include <QObject>
#include <QRegularExpression>
#include <optional>
#include "../model/Entities.h"
#include "../model/Repositories.h"

class AppController : public QObject {
    Q_OBJECT
public:
    static AppController& instance();

    AppController(const AppController&) = delete;
    AppController& operator=(const AppController&) = delete;

    bool login(const QString& username, const QString& password, QString& errorMsg);
    void logout();
    
    bool registerClientAndUser(const Client& client, const QString& login, const QString& password, QString& errorMsg);
    bool updateProfile(const Client& client, const QString& newLogin, const QString& newPassword, QString& errorMsg);

    bool validatePassword(const QString& password, QString& errorMsg);

    bool isLoggedIn() const { return m_isLoggedIn; }
    bool isAdmin() const { return m_roleName == "admin"; }
    User currentUser() const { return m_currentUser; }
    Client currentClient() const { return m_currentClient; }
    QString currentRoleName() const { return m_roleName; }

signals:
    void sessionChanged();

private:
    AppController() = default;
    ~AppController() = default;

    bool m_isLoggedIn = false;
    User m_currentUser;
    Client m_currentClient;
    QString m_roleName;
};

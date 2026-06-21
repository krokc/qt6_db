#include "AppController.h"

AppController& AppController::instance() {
    static AppController inst;
    return inst;
}

bool AppController::login(const QString& username, const QString& password, QString& errorMsg) {
    auto userOpt = UserRepository::getByLogin(username);
    if (!userOpt.has_value()) {
        errorMsg = "Неверный логин или пароль.";
        return false;
    }

    User user = userOpt.value();
    if (user.password != password) {
        errorMsg = "Неверный логин или пароль.";
        return false;
    }

    auto clientOpt = ClientRepository::getById(user.clientId);
    if (!clientOpt.has_value()) {
        errorMsg = "Данные клиента не найдены.";
        return false;
    }

    m_currentUser = user;
    m_currentClient = clientOpt.value();
    m_roleName = UserRepository::getUserRoleName(user.userId);
    m_isLoggedIn = true;

    emit sessionChanged();
    return true;
}

void AppController::logout() {
    m_isLoggedIn = false;
    m_currentUser = User();
    m_currentClient = Client();
    m_roleName = "";
    emit sessionChanged();
}

bool AppController::validatePassword(const QString& password, QString& errorMsg) {
    if (password.length() < 8) {
        errorMsg = "Пароль должен содержать не менее 8 символов.";
        return false;
    }


    QRegularExpression regex("^(?=.*\\d)(?=.*[A-ZА-ЯЁ])(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?~`]).{8,}$");

    if (!regex.match(password).hasMatch()) {
        errorMsg = "Пароль должен быть сложным: содержать не менее одной цифры, одной заглавной буквы и одного специального символа.";
        return false;
    }

    return true;
}


bool AppController::registerClientAndUser(const Client& clientData, const QString& login, const QString& password, QString& errorMsg) {
    if (!validatePassword(password, errorMsg)) {
        return false;
    }

    if (UserRepository::getByLogin(login).has_value()) {
        errorMsg = "Пользователь с таким логином уже существует.";
        return false;
    }

    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.transaction()) {
        errorMsg = "Ошибка базы данных при открытии транзакции.";
        return false;
    }

    Client client = clientData;
    if (!ClientRepository::insert(client)) {
        db.rollback();
        errorMsg = "Не удалось сохранить данные клиента. Телефон или паспорт могут быть уже зарегистрирнужнованы.";
        return false;
    }

    User user;
    user.clientId = client.clientId;
    user.login = login;
    user.password = password;

    if (!UserRepository::insert(user)) {
        db.rollback();
        errorMsg = "Не удалось создать учетную запись пользователя.";
        return false;
    }

    // Role client is 2
    if (!UserRepository::setUserRole(user.userId, 2)) {
        db.rollback();
        errorMsg = "Не удалось установить роль пользователя.";
        return false;
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "Не удалось завершить транзакцию.";
        return false;
    }

    return true;
}

bool AppController::updateProfile(const Client& clientData, const QString& newLogin, const QString& newPassword, QString& errorMsg) {
    if (!newPassword.isEmpty() && !validatePassword(newPassword, errorMsg)) {
        return false;
    }

    if (newLogin != m_currentUser.login) {
        auto existing = UserRepository::getByLogin(newLogin);
        if (existing.has_value() && existing->userId != m_currentUser.userId) {
            errorMsg = "Пользователь с таким логином уже существует.";
            return false;
        }
    }

    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.transaction()) {
        errorMsg = "Ошибка базы данных при открытии транзакции.";
        return false;
    }

    if (!ClientRepository::update(clientData)) {
        db.rollback();
        errorMsg = "Не удалось обновить личные данные.";
        return false;
    }

    User user = m_currentUser;
    user.login = newLogin;
    if (!newPassword.isEmpty()) {
        user.password = newPassword;
    }

    if (!UserRepository::update(user)) {
        db.rollback();
        errorMsg = "Не удалось обновить данные авторизации.";
        return false;
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "Не удалось зафиксировать изменения.";
        return false;
    }

    m_currentUser = user;
    m_currentClient = clientData;
    emit sessionChanged();
    return true;
}

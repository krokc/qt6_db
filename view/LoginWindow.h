#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "../controller/AppController.h"

class LoginWindow : public QDialog {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess();
    void openRegistration();

private slots:
    void handleLogin();
    void handleRegister();
    void handleConfigureDb();

private:
    QLineEdit* m_loginEdit;
    QLineEdit* m_passwordEdit;
    QLabel* m_errorLabel;
    QPushButton* m_loginButton;
    QPushButton* m_registerButton;
    QPushButton* m_configDbButton;
};

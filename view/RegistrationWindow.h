#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include "../controller/AppController.h"

class RegistrationWindow : public QDialog {
    Q_OBJECT
public:
    explicit RegistrationWindow(QWidget* parent = nullptr);

private slots:
    void handleRegister();

private:
    QLineEdit* m_lastNameEdit;
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_middleNameEdit;
    QDateEdit* m_birthDateEdit;
    QLineEdit* m_passportSeriesEdit;
    QLineEdit* m_passportNumberEdit;
    QLineEdit* m_passportIssuedByEdit;
    QDateEdit* m_passportIssuedDateEdit;
    QLineEdit* m_addressEdit;
    QLineEdit* m_phoneEdit;
    QLineEdit* m_loginEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_passwordConfirmEdit;
    QLabel* m_errorLabel;
};

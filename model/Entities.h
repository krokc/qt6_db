#pragma once
#include <QString>
#include <QDate>

struct Client {
    int clientId = 0;
    QString lastName;
    QString firstName;
    QString middleName;
    QDate birthDate;
    QString passportSeries;
    QString passportNumber;
    QString passportIssuedBy;
    QDate passportIssuedDate;
    QString address;
    QString phone;

    QString getFullName() const {
        return QString("%1 %2 %3").arg(lastName, firstName, middleName).trimmed();
    }
};

struct Role {
    int roleId = 0;
    QString roleName;
};

struct Item {
    int itemId = 0;
    QString name;
    QString description;
    QString category;
    QString condition;
    QString status;
};

struct User {
    int userId = 0;
    int clientId = 0;
    QString login;
    QString password;
};

struct UserRole {
    int userId = 0;
    int roleId = 0;
};

struct PledgeContract {
    int contractId = 0;
    int clientId = 0;
    QString contractNumber;
    QDate dateConcluded;
    double commission = 0.0;
    QDate returnDate;
    QString contractStatus;
};

struct ContractPosition {
    int positionId = 0;
    int contractId = 0;
    int itemId = 0;
    double estimatedValue = 0.0;
    double loanAmount = 0.0;
};

struct Repayment {
    int repaymentId = 0;
    int positionId = 0;
    QDate repaymentDate;
    double repaymentAmount = 0.0;
    QString paymentMethod;
};

struct Revaluation {
    int revaluationId = 0;
    int itemId = 0;
    QDate changeDate;
    double newPrice = 0.0;
    QString reason;
};

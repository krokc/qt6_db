#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "../model/Entities.h"
#include "../model/Repositories.h"

class ClientDialog : public QDialog {
    Q_OBJECT
public:
    explicit ClientDialog(QWidget* parent = nullptr, const Client* client = nullptr);
    Client getClient() const;
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
    int m_clientId = 0;
};

class ItemDialog : public QDialog {
    Q_OBJECT
public:
    explicit ItemDialog(QWidget* parent = nullptr, const Item* item = nullptr);
    Item getItem() const;
private:
    QLineEdit* m_nameEdit;
    QLineEdit* m_descriptionEdit;
    QLineEdit* m_categoryEdit;
    QLineEdit* m_conditionEdit;
    QComboBox* m_statusCombo;
    int m_itemId = 0;
};

class PledgeContractDialog : public QDialog {
    Q_OBJECT
public:
    explicit PledgeContractDialog(QWidget* parent = nullptr, const PledgeContract* contract = nullptr);
    PledgeContract getContract() const;
private:
    QComboBox* m_clientCombo;
    QLineEdit* m_contractNumberEdit;
    QDateEdit* m_dateConcludedEdit;
    QDoubleSpinBox* m_commissionSpin;
    QDateEdit* m_returnDateEdit;
    QComboBox* m_statusCombo;
    int m_contractId = 0;
};

class ContractPositionDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContractPositionDialog(QWidget* parent = nullptr, const ContractPosition* pos = nullptr);
    ContractPosition getPosition() const;
private:
    QComboBox* m_contractCombo;
    QComboBox* m_itemCombo;
    QDoubleSpinBox* m_estimatedValueSpin;
    QDoubleSpinBox* m_loanAmountSpin;
    int m_positionId = 0;
};

class RepaymentDialog : public QDialog {
    Q_OBJECT
public:
    explicit RepaymentDialog(QWidget* parent = nullptr, const Repayment* rep = nullptr);
    Repayment getRepayment() const;
private:
    QComboBox* m_positionCombo;
    QDateEdit* m_repaymentDateEdit;
    QDoubleSpinBox* m_repaymentAmountSpin;
    QLineEdit* m_paymentMethodEdit;
    int m_repaymentId = 0;
};

class RevaluationDialog : public QDialog {
    Q_OBJECT
public:
    explicit RevaluationDialog(QWidget* parent = nullptr, const Revaluation* rev = nullptr);
    Revaluation getRevaluation() const;
private:
    QComboBox* m_itemCombo;
    QDateEdit* m_changeDateEdit;
    QDoubleSpinBox* m_newPriceSpin;
    QLineEdit* m_reasonEdit;
    int m_revaluationId = 0;
};

class UserDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserDialog(QWidget* parent = nullptr, const User* user = nullptr);
    User getUser() const;
    int getRoleId() const;
private:
    QComboBox* m_clientCombo;
    QLineEdit* m_loginEdit;
    QLineEdit* m_passwordEdit;
    QComboBox* m_roleCombo;
    int m_userId = 0;
};

#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QFormLayout>
#include "../controller/AppController.h"
#include "CrudDialogs.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void handleLogout();
    void refreshAllTables();
    void setupUiForRole();

    void handleUpdateProfile();

    void addClient();
    void editClient();
    void deleteClient();
    
    void addItem();
    void editItem();
    void deleteItem();

    void addContract();
    void editContract();
    void deleteContract();

    void addPosition();
    void editPosition();
    void deletePosition();

    void addRepayment();
    void editRepayment();
    void deleteRepayment();

    void addRevaluation();
    void editRevaluation();
    void deleteRevaluation();

    void addUser();
    void editUser();
    void deleteUser();

private:
    void setupAdminUi();
    void setupClientUi();

    void refreshClientsTable();
    void refreshItemsTable();
    void refreshContractsTable();
    void refreshPositionsTable();
    void refreshRepaymentsTable();
    void refreshRevaluationsTable();
    void refreshUsersTable();

    QTabWidget* m_tabWidget;

    QTableWidget* m_clientsTable = nullptr;
    QTableWidget* m_itemsTable = nullptr;
    QTableWidget* m_contractsTable = nullptr;
    QTableWidget* m_positionsTable = nullptr;
    QTableWidget* m_repaymentsTable = nullptr;
    QTableWidget* m_revaluationsTable = nullptr;
    QTableWidget* m_usersTable = nullptr;

    QLineEdit* m_pLastNameEdit = nullptr;
    QLineEdit* m_pFirstNameEdit = nullptr;
    QLineEdit* m_pMiddleNameEdit = nullptr;
    QDateEdit* m_pBirthDateEdit = nullptr;
    QLineEdit* m_pPassportSeriesEdit = nullptr;
    QLineEdit* m_pPassportNumberEdit = nullptr;
    QLineEdit* m_pPassportIssuedByEdit = nullptr;
    QDateEdit* m_pPassportIssuedDateEdit = nullptr;
    QLineEdit* m_pPhoneEdit = nullptr;
    QLineEdit* m_pAddressEdit = nullptr;
    QLineEdit* m_pLoginEdit = nullptr;
    QLineEdit* m_pPasswordEdit = nullptr;
};

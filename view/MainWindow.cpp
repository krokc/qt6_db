#include "MainWindow.h"
#include <QScrollArea>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Система управления ломбардом");
    setMinimumSize(850, 600);

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    connect(&AppController::instance(), &AppController::sessionChanged, this, &MainWindow::setupUiForRole);
    setupUiForRole();
}

void MainWindow::setupUiForRole() {
    m_tabWidget->clear();

    if (!AppController::instance().isLoggedIn()) {
        return;
    }

    QWidget* titleBar = new QWidget(this);
    titleBar->setStyleSheet("background-color: #1a1a20; border-bottom: 1px solid #2e2e38;");
    QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(15, 10, 15, 10);

    QLabel* welcomeLabel = new QLabel(QString("Пользователь: <b>%1</b> (%2)")
        .arg(AppController::instance().currentUser().login)
        .arg(AppController::instance().currentRoleName() == "admin" ? "Администратор" : "Клиент"), this);
    QPushButton* logoutBtn = new QPushButton("Выйти", this);
    logoutBtn->setObjectName("secondaryButton");
    logoutBtn->setFixedWidth(100);

    titleLayout->addWidget(welcomeLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(logoutBtn);

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);

    QWidget* mainContainer = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);

    m_tabWidget->setParent(mainContainer);
    mainLayout->addWidget(m_tabWidget);

    setCentralWidget(mainContainer);

    if (AppController::instance().isAdmin()) {
        setupAdminUi();
    } else {
        setupClientUi();
    }
    refreshAllTables();
}

void MainWindow::handleLogout() {
    AppController::instance().logout();
    close();
}

void MainWindow::refreshAllTables() {
    if (AppController::instance().isAdmin()) {
        refreshClientsTable();
        refreshItemsTable();
        refreshContractsTable();
        refreshPositionsTable();
        refreshRepaymentsTable();
        refreshRevaluationsTable();
        refreshUsersTable();
    } else {
        refreshContractsTable();
        refreshPositionsTable();
        refreshRepaymentsTable();
        refreshRevaluationsTable();
        
        Client c = AppController::instance().currentClient();
        User u = AppController::instance().currentUser();
        if (m_pLastNameEdit) {
            m_pLastNameEdit->setText(c.lastName);
            m_pFirstNameEdit->setText(c.firstName);
            m_pMiddleNameEdit->setText(c.middleName);
            m_pBirthDateEdit->setDate(c.birthDate);
            m_pPassportSeriesEdit->setText(c.passportSeries);
            m_pPassportNumberEdit->setText(c.passportNumber);
            m_pPassportIssuedByEdit->setText(c.passportIssuedBy);
            m_pPassportIssuedDateEdit->setDate(c.passportIssuedDate);
            m_pPhoneEdit->setText(c.phone);
            m_pAddressEdit->setText(c.address);
            m_pLoginEdit->setText(u.login);
            m_pPasswordEdit->clear();
        }
    }
}


void MainWindow::setupAdminUi() {
    QWidget* clientsTab = new QWidget(m_tabWidget);
    QVBoxLayout* clientsLayout = new QVBoxLayout(clientsTab);
    m_clientsTable = new QTableWidget(clientsTab);
    m_clientsTable->setColumnCount(11);
    m_clientsTable->setHorizontalHeaderLabels({"ID", "Фамилия", "Имя", "Отчество", "ДР", "Серия", "Номер", "Выдан", "Дата выдачи", "Адрес", "Телефон"});
    m_clientsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_clientsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* clientsBtns = new QHBoxLayout();
    QPushButton* addC = new QPushButton("Добавить", clientsTab);
    QPushButton* editC = new QPushButton("Редактировать", clientsTab);
    QPushButton* delC = new QPushButton("Удалить", clientsTab);
    delC->setObjectName("dangerButton");
    clientsBtns->addWidget(addC);
    clientsBtns->addWidget(editC);
    clientsBtns->addWidget(delC);
    clientsBtns->addStretch();

    clientsLayout->addWidget(m_clientsTable);
    clientsLayout->addLayout(clientsBtns);
    m_tabWidget->addTab(clientsTab, "Клиенты");

    connect(addC, &QPushButton::clicked, this, &MainWindow::addClient);
    connect(editC, &QPushButton::clicked, this, &MainWindow::editClient);
    connect(delC, &QPushButton::clicked, this, &MainWindow::deleteClient);

    QWidget* itemsTab = new QWidget(m_tabWidget);
    QVBoxLayout* itemsLayout = new QVBoxLayout(itemsTab);
    m_itemsTable = new QTableWidget(itemsTab);
    m_itemsTable->setColumnCount(6);
    m_itemsTable->setHorizontalHeaderLabels({"ID", "Название", "Описание", "Категория", "Состояние", "Статус"});
    m_itemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* itemsBtns = new QHBoxLayout();
    QPushButton* addI = new QPushButton("Добавить", itemsTab);
    QPushButton* editI = new QPushButton("Редактировать", itemsTab);
    QPushButton* delI = new QPushButton("Удалить", itemsTab);
    delI->setObjectName("dangerButton");
    itemsBtns->addWidget(addI);
    itemsBtns->addWidget(editI);
    itemsBtns->addWidget(delI);
    itemsBtns->addStretch();

    itemsLayout->addWidget(m_itemsTable);
    itemsLayout->addLayout(itemsBtns);
    m_tabWidget->addTab(itemsTab, "Товары");

    connect(addI, &QPushButton::clicked, this, &MainWindow::addItem);
    connect(editI, &QPushButton::clicked, this, &MainWindow::editItem);
    connect(delI, &QPushButton::clicked, this, &MainWindow::deleteItem);

    QWidget* contractsTab = new QWidget(m_tabWidget);
    QVBoxLayout* contractsLayout = new QVBoxLayout(contractsTab);
    m_contractsTable = new QTableWidget(contractsTab);
    m_contractsTable->setColumnCount(7);
    m_contractsTable->setHorizontalHeaderLabels({"ID", "Клиент ID", "Номер договора", "Дата", "Комиссия", "Дата возврата", "Статус"});
    m_contractsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_contractsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_contractsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_contractsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* contractsBtns = new QHBoxLayout();
    QPushButton* addContractBtn = new QPushButton("Новый договор", contractsTab);
    QPushButton* editContractBtn = new QPushButton("Редактировать", contractsTab);
    QPushButton* delContractBtn = new QPushButton("Удалить", contractsTab);
    delContractBtn->setObjectName("dangerButton");
    contractsBtns->addWidget(addContractBtn);
    contractsBtns->addWidget(editContractBtn);
    contractsBtns->addWidget(delContractBtn);
    contractsBtns->addStretch();

    contractsLayout->addWidget(m_contractsTable);
    contractsLayout->addLayout(contractsBtns);
    m_tabWidget->addTab(contractsTab, "Договоры");

    connect(addContractBtn, &QPushButton::clicked, this, &MainWindow::addContract);
    connect(editContractBtn, &QPushButton::clicked, this, &MainWindow::editContract);
    connect(delContractBtn, &QPushButton::clicked, this, &MainWindow::deleteContract);

    QWidget* posTab = new QWidget(m_tabWidget);
    QVBoxLayout* posLayout = new QVBoxLayout(posTab);
    m_positionsTable = new QTableWidget(posTab);
    m_positionsTable->setColumnCount(5);
    m_positionsTable->setHorizontalHeaderLabels({"ID", "Договор ID", "Товар ID", "Оценка", "Сумма займа"});
    m_positionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_positionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_positionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_positionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* posBtns = new QHBoxLayout();
    QPushButton* addP = new QPushButton("Добавить позицию", posTab);
    QPushButton* editP = new QPushButton("Редактировать", posTab);
    QPushButton* delP = new QPushButton("Удалить", posTab);
    delP->setObjectName("dangerButton");
    posBtns->addWidget(addP);
    posBtns->addWidget(editP);
    posBtns->addWidget(delP);
    posBtns->addStretch();

    posLayout->addWidget(m_positionsTable);
    posLayout->addLayout(posBtns);
    m_tabWidget->addTab(posTab, "Позиции");

    connect(addP, &QPushButton::clicked, this, &MainWindow::addPosition);
    connect(editP, &QPushButton::clicked, this, &MainWindow::editPosition);
    connect(delP, &QPushButton::clicked, this, &MainWindow::deletePosition);

    QWidget* repTab = new QWidget(m_tabWidget);
    QVBoxLayout* repLayout = new QVBoxLayout(repTab);
    m_repaymentsTable = new QTableWidget(repTab);
    m_repaymentsTable->setColumnCount(5);
    m_repaymentsTable->setHorizontalHeaderLabels({"ID", "Позиция ID", "Дата платежа", "Сумма", "Способ оплаты"});
    m_repaymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_repaymentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_repaymentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_repaymentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* repBtns = new QHBoxLayout();
    QPushButton* addR = new QPushButton("Оформить возврат", repTab);
    QPushButton* editR = new QPushButton("Редактировать", repTab);
    QPushButton* delR = new QPushButton("Удалить", repTab);
    delR->setObjectName("dangerButton");
    repBtns->addWidget(addR);
    repBtns->addWidget(editR);
    repBtns->addWidget(delR);
    repBtns->addStretch();

    repLayout->addWidget(m_repaymentsTable);
    repLayout->addLayout(repBtns);
    m_tabWidget->addTab(repTab, "Возвраты");

    connect(addR, &QPushButton::clicked, this, &MainWindow::addRepayment);
    connect(editR, &QPushButton::clicked, this, &MainWindow::editRepayment);
    connect(delR, &QPushButton::clicked, this, &MainWindow::deleteRepayment);

    QWidget* revTab = new QWidget(m_tabWidget);
    QVBoxLayout* revLayout = new QVBoxLayout(revTab);
    m_revaluationsTable = new QTableWidget(revTab);
    m_revaluationsTable->setColumnCount(5);
    m_revaluationsTable->setHorizontalHeaderLabels({"ID", "Товар ID", "Дата изменения", "Новая цена", "Причина"});
    m_revaluationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_revaluationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_revaluationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_revaluationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* revBtns = new QHBoxLayout();
    QPushButton* addRev = new QPushButton("Переоценить товар", revTab);
    QPushButton* editRev = new QPushButton("Редактировать", revTab);
    QPushButton* delRev = new QPushButton("Удалить", revTab);
    delRev->setObjectName("dangerButton");
    revBtns->addWidget(addRev);
    revBtns->addWidget(editRev);
    revBtns->addWidget(delRev);
    revBtns->addStretch();

    revLayout->addWidget(m_revaluationsTable);
    revLayout->addLayout(revBtns);
    m_tabWidget->addTab(revTab, "Переоценки");

    connect(addRev, &QPushButton::clicked, this, &MainWindow::addRevaluation);
    connect(editRev, &QPushButton::clicked, this, &MainWindow::editRevaluation);
    connect(delRev, &QPushButton::clicked, this, &MainWindow::deleteRevaluation);

    QWidget* usersTab = new QWidget(m_tabWidget);
    QVBoxLayout* usersLayout = new QVBoxLayout(usersTab);
    m_usersTable = new QTableWidget(usersTab);
    m_usersTable->setColumnCount(4);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Клиент ID", "Логин", "Роль"});
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* usersBtns = new QHBoxLayout();
    QPushButton* addU = new QPushButton("Добавить пользователя", usersTab);
    QPushButton* editU = new QPushButton("Редактировать", usersTab);
    QPushButton* delU = new QPushButton("Удалить", usersTab);
    delU->setObjectName("dangerButton");
    usersBtns->addWidget(addU);
    usersBtns->addWidget(editU);
    usersBtns->addWidget(delU);
    usersBtns->addStretch();

    usersLayout->addWidget(m_usersTable);
    usersLayout->addLayout(usersBtns);
    m_tabWidget->addTab(usersTab, "Пользователи");

    connect(addU, &QPushButton::clicked, this, &MainWindow::addUser);
    connect(editU, &QPushButton::clicked, this, &MainWindow::editUser);
    connect(delU, &QPushButton::clicked, this, &MainWindow::deleteUser);
}

void MainWindow::setupClientUi() {
    QScrollArea* scrollArea = new QScrollArea(m_tabWidget);
    scrollArea->setWidgetResizable(true);
    QWidget* profileTab = new QWidget(scrollArea);
    QFormLayout* form = new QFormLayout(profileTab);
    form->setSpacing(10);
    form->setContentsMargins(20, 20, 20, 20);

    m_pLastNameEdit = new QLineEdit(profileTab);
    m_pFirstNameEdit = new QLineEdit(profileTab);
    m_pMiddleNameEdit = new QLineEdit(profileTab);
    m_pBirthDateEdit = new QDateEdit(profileTab);
    m_pBirthDateEdit->setCalendarPopup(true);
    m_pBirthDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_pPassportSeriesEdit = new QLineEdit(profileTab);
    m_pPassportNumberEdit = new QLineEdit(profileTab);
    m_pPassportIssuedByEdit = new QLineEdit(profileTab);
    m_pPassportIssuedDateEdit = new QDateEdit(profileTab);
    m_pPassportIssuedDateEdit->setCalendarPopup(true);
    m_pPassportIssuedDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_pPhoneEdit = new QLineEdit(profileTab);
    m_pAddressEdit = new QLineEdit(profileTab);

    m_pLoginEdit = new QLineEdit(profileTab);
    m_pPasswordEdit = new QLineEdit(profileTab);
    m_pPasswordEdit->setEchoMode(QLineEdit::Password);
    m_pPasswordEdit->setPlaceholderText("Оставьте пустым, если не хотите менять");

    form->addRow(new QLabel("<h3>Личные данные</h3>", profileTab));
    form->addRow("Фамилия *:", m_pLastNameEdit);
    form->addRow("Имя *:", m_pFirstNameEdit);
    form->addRow("Отчество:", m_pMiddleNameEdit);
    form->addRow("Дата рождения:", m_pBirthDateEdit);
    form->addRow("Серия паспорта *:", m_pPassportSeriesEdit);
    form->addRow("Номер паспорта *:", m_pPassportNumberEdit);
    form->addRow("Кем выдан:", m_pPassportIssuedByEdit);
    form->addRow("Дата выдачи:", m_pPassportIssuedDateEdit);
    form->addRow("Телефон *:", m_pPhoneEdit);
    form->addRow("Адрес проживания:", m_pAddressEdit);

    QFrame* line = new QFrame(profileTab);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #2e2e38; margin: 10px 0;");
    form->addRow(line);

    form->addRow(new QLabel("<h3>Учетные данные</h3>", profileTab));
    form->addRow("Логин пользователя *:", m_pLoginEdit);
    form->addRow("Новый пароль:", m_pPasswordEdit);

    QPushButton* updateBtn = new QPushButton("Обновить данные профиля", profileTab);
    form->addRow(updateBtn);

    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::handleUpdateProfile);

    profileTab->setLayout(form);
    scrollArea->setWidget(profileTab);
    m_tabWidget->addTab(scrollArea, "Мой Профиль");

    QWidget* contractsTab = new QWidget(m_tabWidget);
    QVBoxLayout* contractsLayout = new QVBoxLayout(contractsTab);
    m_contractsTable = new QTableWidget(contractsTab);
    m_contractsTable->setColumnCount(6);
    m_contractsTable->setHorizontalHeaderLabels({"ID", "Номер договора", "Дата", "Комиссия", "Дата возврата", "Статус"});
    m_contractsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_contractsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    contractsLayout->addWidget(m_contractsTable);
    m_tabWidget->addTab(contractsTab, "Мои Договоры");

    QWidget* posTab = new QWidget(m_tabWidget);
    QVBoxLayout* posLayout = new QVBoxLayout(posTab);
    m_positionsTable = new QTableWidget(posTab);
    m_positionsTable->setColumnCount(5);
    m_positionsTable->setHorizontalHeaderLabels({"Позиция ID", "Договор №", "Товар", "Оценка", "Сумма займа"});
    m_positionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_positionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    posLayout->addWidget(m_positionsTable);
    m_tabWidget->addTab(posTab, "Залоги");

    QWidget* repTab = new QWidget(m_tabWidget);
    QVBoxLayout* repLayout = new QVBoxLayout(repTab);
    m_repaymentsTable = new QTableWidget(repTab);
    m_repaymentsTable->setColumnCount(5);
    m_repaymentsTable->setHorizontalHeaderLabels({"Платеж ID", "Договор №", "Дата платежа", "Сумма", "Способ оплаты"});
    m_repaymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_repaymentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    repLayout->addWidget(m_repaymentsTable);
    m_tabWidget->addTab(repTab, "Мои Выплаты");

    QWidget* revTab = new QWidget(m_tabWidget);
    QVBoxLayout* revLayout = new QVBoxLayout(revTab);
    m_revaluationsTable = new QTableWidget(revTab);
    m_revaluationsTable->setColumnCount(5);
    m_revaluationsTable->setHorizontalHeaderLabels({"Переоценка ID", "Товар", "Дата изменения", "Новая цена", "Причина"});
    m_revaluationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_revaluationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    revLayout->addWidget(m_revaluationsTable);
    m_tabWidget->addTab(revTab, "История переоценок");
}

void MainWindow::handleUpdateProfile() {
    Client c = AppController::instance().currentClient();
    c.lastName = m_pLastNameEdit->text().trimmed();
    c.firstName = m_pFirstNameEdit->text().trimmed();
    c.middleName = m_pMiddleNameEdit->text().trimmed();
    c.birthDate = m_pBirthDateEdit->date();
    c.passportSeries = m_pPassportSeriesEdit->text().trimmed();
    c.passportNumber = m_pPassportNumberEdit->text().trimmed();
    c.passportIssuedBy = m_pPassportIssuedByEdit->text().trimmed();
    c.passportIssuedDate = m_pPassportIssuedDateEdit->date();
    c.phone = m_pPhoneEdit->text().trimmed();
    c.address = m_pAddressEdit->text().trimmed();

    QString newLogin = m_pLoginEdit->text().trimmed();
    QString newPassword = m_pPasswordEdit->text();

    if (c.lastName.isEmpty() || c.firstName.isEmpty() || c.passportSeries.isEmpty() ||
        c.passportNumber.isEmpty() || c.phone.isEmpty() || newLogin.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, заполните все обязательные поля.");
        return;
    }

    QString errorMsg;
    if (AppController::instance().updateProfile(c, newLogin, newPassword, errorMsg)) {
        QMessageBox::information(this, "Успех", "Профиль успешно обновлен!");
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Ошибка", errorMsg);
    }
}


void MainWindow::refreshClientsTable() {
    if (!m_clientsTable) return;
    m_clientsTable->setRowCount(0);
    auto list = ClientRepository::getAll();
    for (int r = 0; r < list.size(); ++r) {
        m_clientsTable->insertRow(r);
        m_clientsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].clientId)));
        m_clientsTable->setItem(r, 1, new QTableWidgetItem(list[r].lastName));
        m_clientsTable->setItem(r, 2, new QTableWidgetItem(list[r].firstName));
        m_clientsTable->setItem(r, 3, new QTableWidgetItem(list[r].middleName));
        m_clientsTable->setItem(r, 4, new QTableWidgetItem(list[r].birthDate.toString("dd.MM.yyyy")));
        m_clientsTable->setItem(r, 5, new QTableWidgetItem(list[r].passportSeries));
        m_clientsTable->setItem(r, 6, new QTableWidgetItem(list[r].passportNumber));
        m_clientsTable->setItem(r, 7, new QTableWidgetItem(list[r].passportIssuedBy));
        m_clientsTable->setItem(r, 8, new QTableWidgetItem(list[r].passportIssuedDate.toString("dd.MM.yyyy")));
        m_clientsTable->setItem(r, 9, new QTableWidgetItem(list[r].address));
        m_clientsTable->setItem(r, 10, new QTableWidgetItem(list[r].phone));
    }
}

void MainWindow::refreshItemsTable() {
    if (!m_itemsTable) return;
    m_itemsTable->setRowCount(0);
    auto list = ItemRepository::getAll();
    for (int r = 0; r < list.size(); ++r) {
        m_itemsTable->insertRow(r);
        m_itemsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].itemId)));
        m_itemsTable->setItem(r, 1, new QTableWidgetItem(list[r].name));
        m_itemsTable->setItem(r, 2, new QTableWidgetItem(list[r].description));
        m_itemsTable->setItem(r, 3, new QTableWidgetItem(list[r].category));
        m_itemsTable->setItem(r, 4, new QTableWidgetItem(list[r].condition));
        m_itemsTable->setItem(r, 5, new QTableWidgetItem(list[r].status));
    }
}

void MainWindow::refreshContractsTable() {
    if (!m_contractsTable) return;
    m_contractsTable->setRowCount(0);

    QList<PledgeContract> list;
    if (AppController::instance().isAdmin()) {
        list = PledgeContractRepository::getAll();
    } else {
        list = PledgeContractRepository::getByClientId(AppController::instance().currentClient().clientId);
    }

    for (int r = 0; r < list.size(); ++r) {
        m_contractsTable->insertRow(r);
        if (AppController::instance().isAdmin()) {
            m_contractsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].contractId)));
            m_contractsTable->setItem(r, 1, new QTableWidgetItem(QString::number(list[r].clientId)));
            m_contractsTable->setItem(r, 2, new QTableWidgetItem(list[r].contractNumber));
            m_contractsTable->setItem(r, 3, new QTableWidgetItem(list[r].dateConcluded.toString("dd.MM.yyyy")));
            m_contractsTable->setItem(r, 4, new QTableWidgetItem(QString::number(list[r].commission, 'f', 2)));
            m_contractsTable->setItem(r, 5, new QTableWidgetItem(list[r].returnDate.toString("dd.MM.yyyy")));
            m_contractsTable->setItem(r, 6, new QTableWidgetItem(list[r].contractStatus));
        } else {
            m_contractsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].contractId)));
            m_contractsTable->setItem(r, 1, new QTableWidgetItem(list[r].contractNumber));
            m_contractsTable->setItem(r, 2, new QTableWidgetItem(list[r].dateConcluded.toString("dd.MM.yyyy")));
            m_contractsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].commission, 'f', 2) + " %"));
            m_contractsTable->setItem(r, 4, new QTableWidgetItem(list[r].returnDate.toString("dd.MM.yyyy")));
            m_contractsTable->setItem(r, 5, new QTableWidgetItem(list[r].contractStatus));
        }
    }
}

void MainWindow::refreshPositionsTable() {
    if (!m_positionsTable) return;
    m_positionsTable->setRowCount(0);

    QList<ContractPosition> list;
    if (AppController::instance().isAdmin()) {
        list = ContractPositionRepository::getAll();
    } else {
        auto contracts = PledgeContractRepository::getByClientId(AppController::instance().currentClient().clientId);
        for (const auto& c : contracts) {
            list.append(ContractPositionRepository::getByContractId(c.contractId));
        }
    }

    for (int r = 0; r < list.size(); ++r) {
        m_positionsTable->insertRow(r);
        if (AppController::instance().isAdmin()) {
            m_positionsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].positionId)));
            m_positionsTable->setItem(r, 1, new QTableWidgetItem(QString::number(list[r].contractId)));
            m_positionsTable->setItem(r, 2, new QTableWidgetItem(QString::number(list[r].itemId)));
            m_positionsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].estimatedValue, 'f', 2)));
            m_positionsTable->setItem(r, 4, new QTableWidgetItem(QString::number(list[r].loanAmount, 'f', 2)));
        } else {
            auto contrOpt = PledgeContractRepository::getById(list[r].contractId);
            auto itemOpt = ItemRepository::getById(list[r].itemId);
            QString contrNum = contrOpt ? contrOpt->contractNumber : QString::number(list[r].contractId);
            QString itemName = itemOpt ? itemOpt->name : QString::number(list[r].itemId);

            m_positionsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].positionId)));
            m_positionsTable->setItem(r, 1, new QTableWidgetItem(contrNum));
            m_positionsTable->setItem(r, 2, new QTableWidgetItem(itemName));
            m_positionsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].estimatedValue, 'f', 2)));
            m_positionsTable->setItem(r, 4, new QTableWidgetItem(QString::number(list[r].loanAmount, 'f', 2)));
        }
    }
}

void MainWindow::refreshRepaymentsTable() {
    if (!m_repaymentsTable) return;
    m_repaymentsTable->setRowCount(0);

    QList<Repayment> list;
    if (AppController::instance().isAdmin()) {
        list = RepaymentRepository::getAll();
    } else {
        auto contracts = PledgeContractRepository::getByClientId(AppController::instance().currentClient().clientId);
        for (const auto& c : contracts) {
            auto positions = ContractPositionRepository::getByContractId(c.contractId);
            for (const auto& p : positions) {
                list.append(RepaymentRepository::getByPositionId(p.positionId));
            }
        }
    }

    for (int r = 0; r < list.size(); ++r) {
        m_repaymentsTable->insertRow(r);
        if (AppController::instance().isAdmin()) {
            m_repaymentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].repaymentId)));
            m_repaymentsTable->setItem(r, 1, new QTableWidgetItem(QString::number(list[r].positionId)));
            m_repaymentsTable->setItem(r, 2, new QTableWidgetItem(list[r].repaymentDate.toString("dd.MM.yyyy")));
            m_repaymentsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].repaymentAmount, 'f', 2)));
            m_repaymentsTable->setItem(r, 4, new QTableWidgetItem(list[r].paymentMethod));
        } else {
            auto posOpt = ContractPositionRepository::getById(list[r].positionId);
            QString contrNum = "Неизвестно";
            if (posOpt) {
                auto contrOpt = PledgeContractRepository::getById(posOpt->contractId);
                if (contrOpt) contrNum = contrOpt->contractNumber;
            }

            m_repaymentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].repaymentId)));
            m_repaymentsTable->setItem(r, 1, new QTableWidgetItem(contrNum));
            m_repaymentsTable->setItem(r, 2, new QTableWidgetItem(list[r].repaymentDate.toString("dd.MM.yyyy")));
            m_repaymentsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].repaymentAmount, 'f', 2)));
            m_repaymentsTable->setItem(r, 4, new QTableWidgetItem(list[r].paymentMethod));
        }
    }
}

void MainWindow::refreshRevaluationsTable() {
    if (!m_revaluationsTable) return;
    m_revaluationsTable->setRowCount(0);

    QList<Revaluation> list;
    if (AppController::instance().isAdmin()) {
        list = RevaluationRepository::getAll();
    } else {
        auto contracts = PledgeContractRepository::getByClientId(AppController::instance().currentClient().clientId);
        for (const auto& c : contracts) {
            auto positions = ContractPositionRepository::getByContractId(c.contractId);
            for (const auto& p : positions) {
                list.append(RevaluationRepository::getByItemId(p.itemId));
            }
        }
    }

    for (int r = 0; r < list.size(); ++r) {
        m_revaluationsTable->insertRow(r);
        if (AppController::instance().isAdmin()) {
            m_revaluationsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].revaluationId)));
            m_revaluationsTable->setItem(r, 1, new QTableWidgetItem(QString::number(list[r].itemId)));
            m_revaluationsTable->setItem(r, 2, new QTableWidgetItem(list[r].changeDate.toString("dd.MM.yyyy")));
            m_revaluationsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].newPrice, 'f', 2)));
            m_revaluationsTable->setItem(r, 4, new QTableWidgetItem(list[r].reason));
        } else {
            auto itemOpt = ItemRepository::getById(list[r].itemId);
            QString itemName = itemOpt ? itemOpt->name : QString::number(list[r].itemId);

            m_revaluationsTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].revaluationId)));
            m_revaluationsTable->setItem(r, 1, new QTableWidgetItem(itemName));
            m_revaluationsTable->setItem(r, 2, new QTableWidgetItem(list[r].changeDate.toString("dd.MM.yyyy")));
            m_revaluationsTable->setItem(r, 3, new QTableWidgetItem(QString::number(list[r].newPrice, 'f', 2)));
            m_revaluationsTable->setItem(r, 4, new QTableWidgetItem(list[r].reason));
        }
    }
}

void MainWindow::refreshUsersTable() {
    if (!m_usersTable) return;
    m_usersTable->setRowCount(0);
    auto list = UserRepository::getAll();
    for (int r = 0; r < list.size(); ++r) {
        m_usersTable->insertRow(r);
        m_usersTable->setItem(r, 0, new QTableWidgetItem(QString::number(list[r].userId)));
        m_usersTable->setItem(r, 1, new QTableWidgetItem(QString::number(list[r].clientId)));
        m_usersTable->setItem(r, 2, new QTableWidgetItem(list[r].login));
        m_usersTable->setItem(r, 3, new QTableWidgetItem(UserRepository::getUserRoleName(list[r].userId)));
    }
}


void MainWindow::addClient() {
    ClientDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Client c = dlg.getClient();
        if (ClientRepository::insert(c)) {
            refreshClientsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить клиента.");
        }
    }
}

void MainWindow::editClient() {
    int row = m_clientsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите клиента для редактирования.");
        return;
    }
    int id = m_clientsTable->item(row, 0)->text().toInt();
    auto cOpt = ClientRepository::getById(id);
    if (cOpt) {
        ClientDialog dlg(this, &(*cOpt));
        if (dlg.exec() == QDialog::Accepted) {
            Client updated = dlg.getClient();
            if (ClientRepository::update(updated)) {
                refreshClientsTable();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить клиента.");
            }
        }
    }
}

void MainWindow::deleteClient() {
    int row = m_clientsTable->currentRow();
    if (row < 0) return;
    int id = m_clientsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить этого клиента?") == QMessageBox::Yes) {
        if (ClientRepository::remove(id)) {
            refreshClientsTable();
            refreshContractsTable();
            refreshPositionsTable();
            refreshRepaymentsTable();
            refreshUsersTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить клиента.");
        }
    }
}

void MainWindow::addItem() {
    ItemDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Item i = dlg.getItem();
        if (ItemRepository::insert(i)) {
            refreshItemsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить товар.");
        }
    }
}

void MainWindow::editItem() {
    int row = m_itemsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите товар для редактирования.");
        return;
    }
    int id = m_itemsTable->item(row, 0)->text().toInt();
    auto iOpt = ItemRepository::getById(id);
    if (iOpt) {
        ItemDialog dlg(this, &(*iOpt));
        if (dlg.exec() == QDialog::Accepted) {
            Item updated = dlg.getItem();
            if (ItemRepository::update(updated)) {
                refreshItemsTable();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить товар.");
            }
        }
    }
}

void MainWindow::deleteItem() {
    int row = m_itemsTable->currentRow();
    if (row < 0) return;
    int id = m_itemsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить этот товар?") == QMessageBox::Yes) {
        if (ItemRepository::remove(id)) {
            refreshItemsTable();
            refreshPositionsTable();
            refreshRepaymentsTable();
            refreshRevaluationsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить товар.");
        }
    }
}

void MainWindow::addContract() {
    PledgeContractDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        PledgeContract pc = dlg.getContract();
        if (PledgeContractRepository::insert(pc)) {
            refreshContractsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить договор (возможно, номер уже существует).");
        }
    }
}

void MainWindow::editContract() {
    int row = m_contractsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите договор.");
        return;
    }
    int id = m_contractsTable->item(row, 0)->text().toInt();
    auto pcOpt = PledgeContractRepository::getById(id);
    if (pcOpt) {
        PledgeContractDialog dlg(this, &(*pcOpt));
        if (dlg.exec() == QDialog::Accepted) {
            PledgeContract updated = dlg.getContract();
            if (PledgeContractRepository::update(updated)) {
                refreshContractsTable();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить договор.");
            }
        }
    }
}

void MainWindow::deleteContract() {
    int row = m_contractsTable->currentRow();
    if (row < 0) return;
    int id = m_contractsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены?") == QMessageBox::Yes) {
        if (PledgeContractRepository::remove(id)) {
            refreshContractsTable();
            refreshPositionsTable();
            refreshRepaymentsTable();
        }
    }
}

void MainWindow::addPosition() {
    ContractPositionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        ContractPosition cp = dlg.getPosition();
        if (ContractPositionRepository::insert(cp)) {
            refreshPositionsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить позицию.");
        }
    }
}

void MainWindow::editPosition() {
    int row = m_positionsTable->currentRow();
    if (row < 0) return;
    int id = m_positionsTable->item(row, 0)->text().toInt();
    auto cpOpt = ContractPositionRepository::getById(id);
    if (cpOpt) {
        ContractPositionDialog dlg(this, &(*cpOpt));
        if (dlg.exec() == QDialog::Accepted) {
            ContractPosition updated = dlg.getPosition();
            if (ContractPositionRepository::update(updated)) {
                refreshPositionsTable();
            }
        }
    }
}

void MainWindow::deletePosition() {
    int row = m_positionsTable->currentRow();
    if (row < 0) return;
    int id = m_positionsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены?") == QMessageBox::Yes) {
        if (ContractPositionRepository::remove(id)) {
            refreshPositionsTable();
            refreshRepaymentsTable();
        }
    }
}

void MainWindow::addRepayment() {
    RepaymentDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Repayment r = dlg.getRepayment();
        if (RepaymentRepository::insert(r)) {
            refreshRepaymentsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось внести платеж.");
        }
    }
}

void MainWindow::editRepayment() {
    int row = m_repaymentsTable->currentRow();
    if (row < 0) return;
    int id = m_repaymentsTable->item(row, 0)->text().toInt();
    auto rOpt = RepaymentRepository::getById(id);
    if (rOpt) {
        RepaymentDialog dlg(this, &(*rOpt));
        if (dlg.exec() == QDialog::Accepted) {
            Repayment updated = dlg.getRepayment();
            if (RepaymentRepository::update(updated)) {
                refreshRepaymentsTable();
            }
        }
    }
}

void MainWindow::deleteRepayment() {
    int row = m_repaymentsTable->currentRow();
    if (row < 0) return;
    int id = m_repaymentsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены?") == QMessageBox::Yes) {
        if (RepaymentRepository::remove(id)) {
            refreshRepaymentsTable();
        }
    }
}

void MainWindow::addRevaluation() {
    RevaluationDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Revaluation r = dlg.getRevaluation();
        if (RevaluationRepository::insert(r)) {
            refreshRevaluationsTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить переоценку.");
        }
    }
}

void MainWindow::editRevaluation() {
    int row = m_revaluationsTable->currentRow();
    if (row < 0) return;
    int id = m_revaluationsTable->item(row, 0)->text().toInt();
    auto revOpt = RevaluationRepository::getById(id);
    if (revOpt) {
        RevaluationDialog dlg(this, &(*revOpt));
        if (dlg.exec() == QDialog::Accepted) {
            Revaluation updated = dlg.getRevaluation();
            if (RevaluationRepository::update(updated)) {
                refreshRevaluationsTable();
            }
        }
    }
}

void MainWindow::deleteRevaluation() {
    int row = m_revaluationsTable->currentRow();
    if (row < 0) return;
    int id = m_revaluationsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Удаление", "Вы уверены?") == QMessageBox::Yes) {
        if (RevaluationRepository::remove(id)) {
            refreshRevaluationsTable();
        }
    }
}

void MainWindow::addUser() {
    UserDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        User u = dlg.getUser();
        int rId = dlg.getRoleId();
        if (UserRepository::insert(u)) {
            UserRepository::setUserRole(u.userId, rId);
            refreshUsersTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить пользователя (логин может быть занят).");
        }
    }
}

void MainWindow::editUser() {
    int row = m_usersTable->currentRow();
    if (row < 0) return;
    int id = m_usersTable->item(row, 0)->text().toInt();
    auto uOpt = UserRepository::getById(id);
    if (uOpt) {
        UserDialog dlg(this, &(*uOpt));
        if (dlg.exec() == QDialog::Accepted) {
            User updated = dlg.getUser();
            int rId = dlg.getRoleId();
            // If password was left blank, retain old one
            if (updated.password.isEmpty()) {
                updated.password = uOpt->password;
            }
            if (UserRepository::update(updated)) {
                UserRepository::setUserRole(updated.userId, rId);
                refreshUsersTable();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить пользователя.");
            }
        }
    }
}

void MainWindow::deleteUser() {
    int row = m_usersTable->currentRow();
    if (row < 0) return;
    int id = m_usersTable->item(row, 0)->text().toInt();
    if (id == AppController::instance().currentUser().userId) {
        QMessageBox::warning(this, "Внимание", "Вы не можете удалить самого себя.");
        return;
    }
    if (QMessageBox::question(this, "Удаление", "Удалить пользователя?") == QMessageBox::Yes) {
        if (UserRepository::remove(id)) {
            refreshUsersTable();
        }
    }
}

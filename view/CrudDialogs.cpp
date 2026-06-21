#include "CrudDialogs.h"
#include "../controller/AppController.h"


ClientDialog::ClientDialog(QWidget *parent, const Client *client) : QDialog(parent) {
    setWindowTitle(client ? "Редактировать клиента" : "Добавить клиента");

    QFormLayout *form = new QFormLayout(this);
    form->setSpacing(10);

    m_lastNameEdit = new QLineEdit(this);
    m_firstNameEdit = new QLineEdit(this);
    m_middleNameEdit = new QLineEdit(this);
    m_birthDateEdit = new QDateEdit(QDate::currentDate().addYears(-18), this);
    m_birthDateEdit->setCalendarPopup(true);
    m_birthDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_passportSeriesEdit = new QLineEdit(this);
    m_passportNumberEdit = new QLineEdit(this);
    m_passportIssuedByEdit = new QLineEdit(this);
    m_passportIssuedDateEdit = new QDateEdit(QDate::currentDate().addYears(-5), this);
    m_passportIssuedDateEdit->setCalendarPopup(true);
    m_passportIssuedDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_addressEdit = new QLineEdit(this);
    m_phoneEdit = new QLineEdit(this);

    form->addRow("Фамилия:", m_lastNameEdit);
    form->addRow("Имя:", m_firstNameEdit);
    form->addRow("Отчество:", m_middleNameEdit);
    form->addRow("Дата рождения:", m_birthDateEdit);
    form->addRow("Серия паспорта:", m_passportSeriesEdit);
    form->addRow("Номер паспорта:", m_passportNumberEdit);
    form->addRow("Кем выдан:", m_passportIssuedByEdit);
    form->addRow("Дата выдачи:", m_passportIssuedDateEdit);
    form->addRow("Адрес:", m_addressEdit);
    form->addRow("Телефон:", m_phoneEdit);

    if (client) {
        m_clientId = client->clientId;
        m_lastNameEdit->setText(client->lastName);
        m_firstNameEdit->setText(client->firstName);
        m_middleNameEdit->setText(client->middleName);
        m_birthDateEdit->setDate(client->birthDate);
        m_passportSeriesEdit->setText(client->passportSeries);
        m_passportNumberEdit->setText(client->passportNumber);
        m_passportIssuedByEdit->setText(client->passportIssuedBy);
        m_passportIssuedDateEdit->setDate(client->passportIssuedDate);
        m_addressEdit->setText(client->address);
        m_phoneEdit->setText(client->phone);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_lastNameEdit->text().trimmed().isEmpty() || m_firstNameEdit->text().trimmed().isEmpty() ||
            m_passportSeriesEdit->text().trimmed().isEmpty() || m_passportNumberEdit->text().trimmed().isEmpty() ||
            m_phoneEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Пожалуйста, заполните Фамилию, Имя, Паспортные данные и Телефон.");
            return;
        }
        accept();
    });

    setMinimumWidth(350);
    adjustSize();
}

Client ClientDialog::getClient() const {
    Client c;
    c.clientId = m_clientId;
    c.lastName = m_lastNameEdit->text().trimmed();
    c.firstName = m_firstNameEdit->text().trimmed();
    c.middleName = m_middleNameEdit->text().trimmed();
    c.birthDate = m_birthDateEdit->date();
    c.passportSeries = m_passportSeriesEdit->text().trimmed();
    c.passportNumber = m_passportNumberEdit->text().trimmed();
    c.passportIssuedBy = m_passportIssuedByEdit->text().trimmed();
    c.passportIssuedDate = m_passportIssuedDateEdit->date();
    c.address = m_addressEdit->text().trimmed();
    c.phone = m_phoneEdit->text().trimmed();
    return c;
}


ItemDialog::ItemDialog(QWidget *parent, const Item *item) : QDialog(parent) {
    setWindowTitle(item ? "Редактировать товар" : "Добавить товар");

    QFormLayout *form = new QFormLayout(this);
    m_nameEdit = new QLineEdit(this);
    m_descriptionEdit = new QLineEdit(this);
    m_categoryEdit = new QLineEdit(this);
    m_conditionEdit = new QLineEdit(this);
    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"В залоге", "Продан", "Передан в магазин", "Выкуплен"});

    form->addRow("Наименование товара:", m_nameEdit);
    form->addRow("Описание:", m_descriptionEdit);
    form->addRow("Категория:", m_categoryEdit);
    form->addRow("Состояние:", m_conditionEdit);
    form->addRow("Статус:", m_statusCombo);

    if (item) {
        m_itemId = item->itemId;
        m_nameEdit->setText(item->name);
        m_descriptionEdit->setText(item->description);
        m_categoryEdit->setText(item->category);
        m_conditionEdit->setText(item->condition);
        m_statusCombo->setCurrentText(item->status);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Наименование товара обязательно.");
            return;
        }
        accept();
    });

    setMinimumWidth(300);
    adjustSize();
}

Item ItemDialog::getItem() const {
    Item i;
    i.itemId = m_itemId;
    i.name = m_nameEdit->text().trimmed();
    i.description = m_descriptionEdit->text().trimmed();
    i.category = m_categoryEdit->text().trimmed();
    i.condition = m_conditionEdit->text().trimmed();
    i.status = m_statusCombo->currentText();
    return i;
}


PledgeContractDialog::PledgeContractDialog(QWidget *parent, const PledgeContract *contract) : QDialog(parent) {
    setWindowTitle(contract ? "Редактировать договор" : "Новый договор залога");

    QFormLayout *form = new QFormLayout(this);

    m_clientCombo = new QComboBox(this);
    auto clients = ClientRepository::getAll();
    for (const auto &c: clients) {
        m_clientCombo->addItem(QString("%1 (ID: %2)").arg(c.getFullName()).arg(c.clientId), c.clientId);
    }

    m_contractNumberEdit = new QLineEdit(this);
    m_contractNumberEdit->setPlaceholderText("ДЗ-XXXX");
    m_dateConcludedEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateConcludedEdit->setCalendarPopup(true);
    m_dateConcludedEdit->setDisplayFormat("dd.MM.yyyy");

    m_commissionSpin = new QDoubleSpinBox(this);
    m_commissionSpin->setRange(0, 100);
    m_commissionSpin->setSuffix(" %");
    m_commissionSpin->setValue(5.0);

    m_returnDateEdit = new QDateEdit(QDate::currentDate().addDays(30), this);
    m_returnDateEdit->setCalendarPopup(true);
    m_returnDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"Активен", "Закрыт", "Просрочен"});

    form->addRow("Клиент:", m_clientCombo);
    form->addRow("Номер договора:", m_contractNumberEdit);
    form->addRow("Дата заключения:", m_dateConcludedEdit);
    form->addRow("Комиссия:", m_commissionSpin);
    form->addRow("Дата возврата:", m_returnDateEdit);
    form->addRow("Статус договора:", m_statusCombo);

    if (contract) {
        m_contractId = contract->contractId;
        int clientIndex = m_clientCombo->findData(contract->clientId);
        if (clientIndex >= 0) m_clientCombo->setCurrentIndex(clientIndex);
        m_contractNumberEdit->setText(contract->contractNumber);
        m_dateConcludedEdit->setDate(contract->dateConcluded);
        m_commissionSpin->setValue(contract->commission);
        m_returnDateEdit->setDate(contract->returnDate);
        m_statusCombo->setCurrentText(contract->contractStatus);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_contractNumberEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Номер договора обязателен.");
            return;
        }
        accept();
    });

    setMinimumWidth(350);
    adjustSize();
}

PledgeContract PledgeContractDialog::getContract() const {
    PledgeContract pc;
    pc.contractId = m_contractId;
    pc.clientId = m_clientCombo->currentData().toInt();
    pc.contractNumber = m_contractNumberEdit->text().trimmed();
    pc.dateConcluded = m_dateConcludedEdit->date();
    pc.commission = m_commissionSpin->value();
    pc.returnDate = m_returnDateEdit->date();
    pc.contractStatus = m_statusCombo->currentText();
    return pc;
}


ContractPositionDialog::ContractPositionDialog(QWidget *parent, const ContractPosition *pos) : QDialog(parent) {
    setWindowTitle(pos ? "Редактировать позицию" : "Добавить позицию в договор");

    QFormLayout *form = new QFormLayout(this);

    m_contractCombo = new QComboBox(this);
    auto contracts = PledgeContractRepository::getAll();
    for (const auto &c: contracts) {
        m_contractCombo->addItem(QString("Договор №%1").arg(c.contractNumber), c.contractId);
    }

    m_itemCombo = new QComboBox(this);
    auto items = ItemRepository::getAll();
    for (const auto &i: items) {
        m_itemCombo->addItem(QString("%1 (ID: %2, %3)").arg(i.name).arg(i.itemId).arg(i.status), i.itemId);
    }

    m_estimatedValueSpin = new QDoubleSpinBox(this);
    m_estimatedValueSpin->setRange(0, 10000000);
    m_estimatedValueSpin->setValue(1000.00);

    m_loanAmountSpin = new QDoubleSpinBox(this);
    m_loanAmountSpin->setRange(0, 10000000);
    m_loanAmountSpin->setValue(800.00);

    form->addRow("Договор залога:", m_contractCombo);
    form->addRow("Товар:", m_itemCombo);
    form->addRow("Оценочная стоимость:", m_estimatedValueSpin);
    form->addRow("Сумма займа:", m_loanAmountSpin);

    if (pos) {
        m_positionId = pos->positionId;
        int cIdx = m_contractCombo->findData(pos->contractId);
        if (cIdx >= 0) m_contractCombo->setCurrentIndex(cIdx);
        int iIdx = m_itemCombo->findData(pos->itemId);
        if (iIdx >= 0) m_itemCombo->setCurrentIndex(iIdx);
        m_estimatedValueSpin->setValue(pos->estimatedValue);
        m_loanAmountSpin->setValue(pos->loanAmount);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_contractCombo->currentIndex() == -1 || m_itemCombo->currentIndex() == -1) {
            QMessageBox::warning(this, "Внимание", "Необходимо выбрать договор и товар.");
            return;
        }
        if (m_loanAmountSpin->value() > m_estimatedValueSpin->value()) {
            QMessageBox::warning(this, "Ошибка", "Сумма займа не может быть больше оценочной стоимости.");
            return;
        }
        accept();
    });

    setMinimumWidth(350);
    adjustSize();
}

ContractPosition ContractPositionDialog::getPosition() const {
    ContractPosition cp;
    cp.positionId = m_positionId;
    cp.contractId = m_contractCombo->currentData().toInt();
    cp.itemId = m_itemCombo->currentData().toInt();
    cp.estimatedValue = m_estimatedValueSpin->value();
    cp.loanAmount = m_loanAmountSpin->value();
    return cp;
}


RepaymentDialog::RepaymentDialog(QWidget *parent, const Repayment *rep) : QDialog(parent) {
    setWindowTitle(rep ? "Редактировать возврат" : "Оформить возврат средств");

    QFormLayout *form = new QFormLayout(this);

    m_positionCombo = new QComboBox(this);
    auto positions = ContractPositionRepository::getAll();
    for (const auto &p: positions) {
        auto itemOpt = ItemRepository::getById(p.itemId);
        QString itemName = itemOpt ? itemOpt->name : QString("Товар ID %1").arg(p.itemId);
        m_positionCombo->addItem(
            QString("Поз. ID %1 (%2) — Заем: %3").arg(p.positionId).arg(itemName).arg(p.loanAmount), p.positionId);
    }

    m_repaymentDateEdit = new QDateEdit(QDate::currentDate(), this);
    m_repaymentDateEdit->setCalendarPopup(true);
    m_repaymentDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_repaymentAmountSpin = new QDoubleSpinBox(this);
    m_repaymentAmountSpin->setRange(0, 10000000);
    m_repaymentAmountSpin->setValue(100.00);

    m_paymentMethodEdit = new QLineEdit(this);
    m_paymentMethodEdit->setText("Наличные");

    form->addRow("Позиция договора:", m_positionCombo);
    form->addRow("Дата платежа:", m_repaymentDateEdit);
    form->addRow("Сумма платежа:", m_repaymentAmountSpin);
    form->addRow("Способ оплаты:", m_paymentMethodEdit);

    if (rep) {
        m_repaymentId = rep->repaymentId;
        int pIdx = m_positionCombo->findData(rep->positionId);
        if (pIdx >= 0) m_positionCombo->setCurrentIndex(pIdx);
        m_repaymentDateEdit->setDate(rep->repaymentDate);
        m_repaymentAmountSpin->setValue(rep->repaymentAmount);
        m_paymentMethodEdit->setText(rep->paymentMethod);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_positionCombo->currentIndex() == -1) {
            QMessageBox::warning(this, "Внимание", "Необходимо выбрать позицию договора.");
            return;
        }
        accept();
    });

    setMinimumWidth(380);
    adjustSize();
}

Repayment RepaymentDialog::getRepayment() const {
    Repayment r;
    r.repaymentId = m_repaymentId;
    r.positionId = m_positionCombo->currentData().toInt();
    r.repaymentDate = m_repaymentDateEdit->date();
    r.repaymentAmount = m_repaymentAmountSpin->value();
    r.paymentMethod = m_paymentMethodEdit->text().trimmed();
    return r;
}


RevaluationDialog::RevaluationDialog(QWidget *parent, const Revaluation *rev) : QDialog(parent) {
    setWindowTitle(rev ? "Редактировать переоценку" : "Новая переоценка товара");

    QFormLayout *form = new QFormLayout(this);

    m_itemCombo = new QComboBox(this);
    auto items = ItemRepository::getAll();
    for (const auto &i: items) {
        m_itemCombo->addItem(QString("%1 (ID: %2)").arg(i.name).arg(i.itemId), i.itemId);
    }

    m_changeDateEdit = new QDateEdit(QDate::currentDate(), this);
    m_changeDateEdit->setCalendarPopup(true);
    m_changeDateEdit->setDisplayFormat("dd.MM.yyyy");

    m_newPriceSpin = new QDoubleSpinBox(this);
    m_newPriceSpin->setRange(0, 10000000);
    m_newPriceSpin->setValue(1000.00);

    m_reasonEdit = new QLineEdit(this);
    m_reasonEdit->setPlaceholderText("Причина переоценки");

    form->addRow("Товар:", m_itemCombo);
    form->addRow("Дата переоценки:", m_changeDateEdit);
    form->addRow("Новая стоимость:", m_newPriceSpin);
    form->addRow("Причина:", m_reasonEdit);

    if (rev) {
        m_revaluationId = rev->revaluationId;
        int iIdx = m_itemCombo->findData(rev->itemId);
        if (iIdx >= 0) m_itemCombo->setCurrentIndex(iIdx);
        m_changeDateEdit->setDate(rev->changeDate);
        m_newPriceSpin->setValue(rev->newPrice);
        m_reasonEdit->setText(rev->reason);
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_itemCombo->currentIndex() == -1) {
            QMessageBox::warning(this, "Внимание", "Необходимо выбрать товар.");
            return;
        }
        accept();
    });

    setMinimumWidth(350);
    adjustSize();
}

Revaluation RevaluationDialog::getRevaluation() const {
    Revaluation r;
    r.revaluationId = m_revaluationId;
    r.itemId = m_itemCombo->currentData().toInt();
    r.changeDate = m_changeDateEdit->date();
    r.newPrice = m_newPriceSpin->value();
    r.reason = m_reasonEdit->text().trimmed();
    return r;
}


UserDialog::UserDialog(QWidget *parent, const User *user) : QDialog(parent) {
    setWindowTitle(user ? "Редактировать пользователя" : "Добавить пользователя");

    QFormLayout *form = new QFormLayout(this);

    m_clientCombo = new QComboBox(this);
    auto clients = ClientRepository::getAll();
    for (const auto &c: clients) {
        m_clientCombo->addItem(QString("%1 (ID: %2)").arg(c.getFullName()).arg(c.clientId), c.clientId);
    }

    m_loginEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Оставьте пустым для сохранения старого");

    m_roleCombo = new QComboBox(this);
    auto roles = RoleRepository::getAll();
    for (const auto &r: roles) {
        m_roleCombo->addItem(r.roleName, r.roleId);
    }

    form->addRow("Связать с клиентом:", m_clientCombo);
    form->addRow("Логин:", m_loginEdit);
    form->addRow("Пароль:", m_passwordEdit);
    form->addRow("Роль пользователя:", m_roleCombo);

    if (user) {
        m_userId = user->userId;
        int cIdx = m_clientCombo->findData(user->clientId);
        if (cIdx >= 0) m_clientCombo->setCurrentIndex(cIdx);
        m_loginEdit->setText(user->login);

        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare("SELECT role_id FROM User_Role WHERE user_id = :userId");
        query.bindValue(":userId", user->userId);
        if (query.exec() && query.next()) {
            int rId = query.value(0).toInt();
            int rIdx = m_roleCombo->findData(rId);
            if (rIdx >= 0) m_roleCombo->setCurrentIndex(rIdx);
        }
    }

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    form->addRow(buttons);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [this, user]() {
        if (m_loginEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Логин обязателен.");
            return;
        }
        if (!user && m_passwordEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Пароль обязателен для нового пользователя.");
            return;
        }

        if (!m_passwordEdit->text().isEmpty()) {
            QString err;
            if (!AppController::instance().validatePassword(m_passwordEdit->text(), err)) {
                QMessageBox::warning(this, "Ненадежный пароль", err);
                return;
            }
        }
        accept();
    });

    setMinimumWidth(350);
    adjustSize();
}

User UserDialog::getUser() const {
    User u;
    u.userId = m_userId;
    u.clientId = m_clientCombo->currentData().toInt();
    u.login = m_loginEdit->text().trimmed();
    u.password = m_passwordEdit->text();
    return u;
}

int UserDialog::getRoleId() const {
    return m_roleCombo->currentData().toInt();
}

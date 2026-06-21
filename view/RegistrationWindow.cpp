#include "RegistrationWindow.h"
#include <QFormLayout>

RegistrationWindow::RegistrationWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Регистрация нового клиента");
    setMinimumSize(450, 550);
    setModal(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Регистрация", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");
    QWidget* scrollContainer = new QWidget(scrollArea);
    QFormLayout* formLayout = new QFormLayout(scrollContainer);
    formLayout->setSpacing(10);

    m_lastNameEdit = new QLineEdit(scrollContainer);
    m_lastNameEdit->setPlaceholderText("Иванов");
    formLayout->addRow("Фамилия *:", m_lastNameEdit);

    m_firstNameEdit = new QLineEdit(scrollContainer);
    m_firstNameEdit->setPlaceholderText("Иван");
    formLayout->addRow("Имя *:", m_firstNameEdit);

    m_middleNameEdit = new QLineEdit(scrollContainer);
    m_middleNameEdit->setPlaceholderText("Иванович");
    formLayout->addRow("Отчество:", m_middleNameEdit);

    m_birthDateEdit = new QDateEdit(QDate::currentDate().addYears(-18), scrollContainer);
    m_birthDateEdit->setCalendarPopup(true);
    m_birthDateEdit->setDisplayFormat("dd.MM.yyyy");
    formLayout->addRow("Дата рождения *:", m_birthDateEdit);

    m_passportSeriesEdit = new QLineEdit(scrollContainer);
    m_passportSeriesEdit->setPlaceholderText("1234");
    formLayout->addRow("Серия паспорта *:", m_passportSeriesEdit);

    m_passportNumberEdit = new QLineEdit(scrollContainer);
    m_passportNumberEdit->setPlaceholderText("567890");
    formLayout->addRow("Номер паспорта *:", m_passportNumberEdit);

    m_passportIssuedByEdit = new QLineEdit(scrollContainer);
    m_passportIssuedByEdit->setPlaceholderText("УВД г. Москвы");
    formLayout->addRow("Кем выдан паспорт:", m_passportIssuedByEdit);

    m_passportIssuedDateEdit = new QDateEdit(QDate::currentDate().addYears(-2), scrollContainer);
    m_passportIssuedDateEdit->setCalendarPopup(true);
    m_passportIssuedDateEdit->setDisplayFormat("dd.MM.yyyy");
    formLayout->addRow("Дата выдачи паспорта:", m_passportIssuedDateEdit);

    m_phoneEdit = new QLineEdit(scrollContainer);
    m_phoneEdit->setPlaceholderText("+79991234567");
    formLayout->addRow("Телефон *:", m_phoneEdit);

    m_addressEdit = new QLineEdit(scrollContainer);
    m_addressEdit->setPlaceholderText("г. Москва, ул. Ленина, д. 1");
    formLayout->addRow("Адрес проживания:", m_addressEdit);

    QFrame* line = new QFrame(scrollContainer);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #2e2e38; margin: 10px 0;");
    formLayout->addRow(line);

    m_loginEdit = new QLineEdit(scrollContainer);
    m_loginEdit->setPlaceholderText("Логин");
    formLayout->addRow("Логин пользователя *:", m_loginEdit);

    m_passwordEdit = new QLineEdit(scrollContainer);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Пароль");
    formLayout->addRow("Пароль *:", m_passwordEdit);

    m_passwordConfirmEdit = new QLineEdit(scrollContainer);
    m_passwordConfirmEdit->setEchoMode(QLineEdit::Password);
    m_passwordConfirmEdit->setPlaceholderText("Повторите пароль");
    formLayout->addRow("Подтверждение *:", m_passwordConfirmEdit);

    scrollContainer->setLayout(formLayout);
    scrollArea->setWidget(scrollContainer);
    mainLayout->addWidget(scrollArea);

    m_errorLabel = new QLabel("", this);
    m_errorLabel->setStyleSheet("color: #e53935; font-size: 11px;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    mainLayout->addWidget(m_errorLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* registerBtn = new QPushButton("Зарегистрироваться", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("secondaryButton");
    btnLayout->addWidget(registerBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(registerBtn, &QPushButton::clicked, this, &RegistrationWindow::handleRegister);
}

void RegistrationWindow::handleRegister() {
    QString lastName = m_lastNameEdit->text().trimmed();
    QString firstName = m_firstNameEdit->text().trimmed();
    QString middleName = m_middleNameEdit->text().trimmed();
    QDate birthDate = m_birthDateEdit->date();
    QString passportSeries = m_passportSeriesEdit->text().trimmed();
    QString passportNumber = m_passportNumberEdit->text().trimmed();
    QString passportIssuedBy = m_passportIssuedByEdit->text().trimmed();
    QDate passportIssuedDate = m_passportIssuedDateEdit->date();
    QString phone = m_phoneEdit->text().trimmed();
    QString address = m_addressEdit->text().trimmed();
    QString login = m_loginEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirm = m_passwordConfirmEdit->text();

    if (lastName.isEmpty() || firstName.isEmpty() || passportSeries.isEmpty() ||
        passportNumber.isEmpty() || phone.isEmpty() || login.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Пожалуйста, заполните все обязательные поля (*).");
        return;
    }

    if (password != confirm) {
        m_errorLabel->setText("Пароли не совпадают.");
        return;
    }

    Client c;
    c.lastName = lastName;
    c.firstName = firstName;
    c.middleName = middleName;
    c.birthDate = birthDate;
    c.passportSeries = passportSeries;
    c.passportNumber = passportNumber;
    c.passportIssuedBy = passportIssuedBy;
    c.passportIssuedDate = passportIssuedDate;
    c.address = address;
    c.phone = phone;

    QString errorMsg;
    if (AppController::instance().registerClientAndUser(c, login, password, errorMsg)) {
        QMessageBox::information(this, "Регистрация успешна", "Вы успешно зарегистрировались! Теперь вы можете войти в систему.");
        accept();
    } else {
        m_errorLabel->setText(errorMsg);
    }
}

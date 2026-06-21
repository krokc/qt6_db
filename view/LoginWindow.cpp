#include "LoginWindow.h"
#include <QSettings>

class DbConfigDialog : public QDialog {
public:
    explicit DbConfigDialog(QWidget* parent = nullptr) : QDialog(parent) {
        setWindowTitle("Настройки БД");

        QVBoxLayout* layout = new QVBoxLayout(this);

        m_hostEdit = new QLineEdit(this);
        m_portEdit = new QLineEdit(this);
        m_dbNameEdit = new QLineEdit(this);
        m_userEdit = new QLineEdit(this);
        m_passwordEdit = new QLineEdit(this);
        m_passwordEdit->setEchoMode(QLineEdit::Password);

        QString configPath = DatabaseManager::instance().configFilePath();
        QSettings settings(configPath, QSettings::IniFormat);
        m_hostEdit->setText(settings.value("Database/host", "127.0.0.1").toString());
        m_portEdit->setText(settings.value("Database/port", 2568).toString());
        m_dbNameEdit->setText(settings.value("Database/database", "pawnshop").toString());
        m_userEdit->setText(settings.value("Database/username", "postgres").toString());
        m_passwordEdit->setText(settings.value("Database/password", "").toString());

        layout->addWidget(new QLabel("Хост:", this));
        layout->addWidget(m_hostEdit);
        layout->addWidget(new QLabel("Порт:", this));
        layout->addWidget(m_portEdit);
        layout->addWidget(new QLabel("Имя базы данных:", this));
        layout->addWidget(m_dbNameEdit);
        layout->addWidget(new QLabel("Пользователь:", this));
        layout->addWidget(m_userEdit);
        layout->addWidget(new QLabel("Пароль:", this));
        layout->addWidget(m_passwordEdit);

        QHBoxLayout* buttons = new QHBoxLayout();
        QPushButton* saveBtn = new QPushButton("Сохранить", this);
        QPushButton* cancelBtn = new QPushButton("Отмена", this);
        cancelBtn->setObjectName("secondaryButton");
        buttons->addWidget(saveBtn);
        buttons->addWidget(cancelBtn);
        layout->addLayout(buttons);

        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        connect(saveBtn, &QPushButton::clicked, [this, &settings]() {
            settings.setValue("Database/host", m_hostEdit->text().trimmed());
            settings.setValue("Database/port", m_portEdit->text().toInt());
            settings.setValue("Database/database", m_dbNameEdit->text().trimmed());
            settings.setValue("Database/username", m_userEdit->text().trimmed());
            settings.setValue("Database/password", m_passwordEdit->text());
            settings.sync();

            DatabaseManager::instance().disconnectFromDatabase();
            if (DatabaseManager::instance().connectToDatabase()) {
                QMessageBox::information(this, "Успех", "Подключение успешно установлено!");
                accept();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных. Проверьте правильность ввода параметров.");
            }
        });

        setMinimumWidth(300);
        adjustSize();
    }

private:
    QLineEdit* m_hostEdit;
    QLineEdit* m_portEdit;
    QLineEdit* m_dbNameEdit;
    QLineEdit* m_userEdit;
    QLineEdit* m_passwordEdit;
};

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Авторизация — Ломбард");
    setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 40, 30, 40);

    QLabel* titleLabel = new QLabel("Вход в систему", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #ffffff;");
    layout->addWidget(titleLabel);

    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText("Логин");
    layout->addWidget(new QLabel("Логин пользователя:", this));
    layout->addWidget(m_loginEdit);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(new QLabel("Пароль:", this));
    layout->addWidget(m_passwordEdit);

    m_errorLabel = new QLabel("", this);
    m_errorLabel->setStyleSheet("color: #e53935; font-size: 11px;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_errorLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_loginButton = new QPushButton("Войти", this);
    m_registerButton = new QPushButton("Регистрация", this);
    m_registerButton->setObjectName("secondaryButton");
    btnLayout->addWidget(m_loginButton);
    btnLayout->addWidget(m_registerButton);
    layout->addLayout(btnLayout);

    m_configDbButton = new QPushButton("Параметры подключения", this);
    m_configDbButton->setObjectName("secondaryButton");
    layout->addWidget(m_configDbButton);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::handleRegister);
    connect(m_configDbButton, &QPushButton::clicked, this, &LoginWindow::handleConfigureDb);

    setMinimumWidth(360);
    adjustSize();
}

void LoginWindow::handleLogin() {
    QString loginStr = m_loginEdit->text();
    QString passStr = m_passwordEdit->text();

    if (loginStr.isEmpty() || passStr.isEmpty()) {
        m_errorLabel->setText("Пожалуйста, заполните все поля.");
        return;
    }

    if (!DatabaseManager::instance().database().isOpen()) {
        m_errorLabel->setText("Нет соединения с базой данных.");
        return;
    }

    QString errorMsg;
    if (AppController::instance().login(loginStr, passStr, errorMsg)) {
        m_errorLabel->clear();
        accept();
        emit loginSuccess();
    } else {
        m_errorLabel->setText(errorMsg);
    }
}

void LoginWindow::handleRegister() {
    emit openRegistration();
}

void LoginWindow::handleConfigureDb() {
    DbConfigDialog dlg(this);
    dlg.exec();
}

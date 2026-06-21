#include "DatabaseManager.h"

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::DatabaseManager() {
    m_db = QSqlDatabase::addDatabase("QPSQL");
}

DatabaseManager::~DatabaseManager() {
    disconnectFromDatabase();
}

QString DatabaseManager::configFilePath() const {
    QString localPath = QDir::current().filePath("db.ini");
    if (QFile::exists(localPath)) {
        return localPath;
    }
    return QDir(QCoreApplication::applicationDirPath()).filePath("db.ini");
}

bool DatabaseManager::connectToDatabase() {
    QString configPath = configFilePath();
    QSettings settings(configPath, QSettings::IniFormat);

    if (!QFile::exists(configPath)) {
        settings.setValue("Database/host", "127.0.0.1");
        settings.setValue("Database/port", 2568);
        settings.setValue("Database/database", "pawnshop");
        settings.setValue("Database/username", "postgres");
        settings.setValue("Database/password", "");
        settings.sync();
        qDebug() << "Created default db.ini at" << configPath;
    }

    QString host = settings.value("Database/host", "127.0.0.1").toString();
    int port = settings.value("Database/port", 2568).toInt();
    QString dbName = settings.value("Database/database", "pawnshop").toString();
    QString username = settings.value("Database/username", "postgres").toString();
    QString password = settings.value("Database/password", "").toString();

    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(username);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qCritical() << "Database connection failed:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "Successfully connected to database" << dbName << "on" << host << ":" << port;
    return true;
}

void DatabaseManager::disconnectFromDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

QSqlDatabase DatabaseManager::database() const {
    return m_db;
}

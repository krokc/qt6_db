#pragma once
#include <QSqlDatabase>
#include <QSqlError>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFile>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool connectToDatabase();
    void disconnectFromDatabase();
    QSqlDatabase database() const;
    
    QString configFilePath() const;

private:
    DatabaseManager();
    ~DatabaseManager();

    QSqlDatabase m_db;
};

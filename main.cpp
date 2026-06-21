#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "model/DatabaseManager.h"
#include "view/LoginWindow.h"
#include "view/RegistrationWindow.h"
#include "view/MainWindow.h"
#include "controller/AppController.h"


int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    DatabaseManager::instance().connectToDatabase();

    bool keepRunning = true;
    while (keepRunning) {
        LoginWindow loginWin;
        
        QObject::connect(&loginWin, &LoginWindow::openRegistration, [&loginWin]() {
            RegistrationWindow regWin(&loginWin);
            regWin.exec();
        });

        if (loginWin.exec() == QDialog::Accepted) {
            MainWindow mainWin;
            mainWin.show();
            application.exec();

            if (!AppController::instance().isLoggedIn())
                continue;

            keepRunning = false;
        } else
            keepRunning = false;
    }

    return 0;
}
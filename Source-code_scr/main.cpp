// main.cpp - Entry point for the application
#include <QApplication>
#include "mainwindow.h"
#include "userauth.h"
#include "gamelogic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application information
    QApplication::setApplicationName("Advanced Tic Tac Toe");
    QApplication::setOrganizationName("Your University Name");

    // Initialize user authentication system
    UserAuth auth;

    // Create and show the main window
    MainWindow w(&auth);
    w.show();

    return a.exec();
}


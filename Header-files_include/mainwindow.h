// mainwindow.h - Main Window Class
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QGridLayout>
#include <QSlider>
#include <QListWidget>
#include <QJsonObject>

#include "userauth.h"
#include "gamelogic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(UserAuth* auth, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleCellClicked();
    void updateBoard();
    void handleGameEnd(Player winner);
    void showLoginPage();
    void showSignupPage();
    void showMenuPage();
    void showGamePage();
    void showGameModePage();
    void showHistoryPage();
    void handleLogin();
    void handleSignup();
    void handleLogout();
    void startTwoPlayerGame();
    void startAIGame();
    void loadSelectedGame();
    void updateReplay(int value);
    void playNextMove();
    void playPreviousMove();

private:
    Ui::MainWindow *ui;
    UserAuth* userAuth;
    GameLogic* gameLogic;
    QStackedWidget* stackedWidget;

    // Login page widgets
    QWidget* loginPage;
    QLineEdit* loginUsername;
    QLineEdit* loginPassword;
    QPushButton* loginButton;
    QPushButton* goToSignupButton;
    QLabel* loginStatusLabel;

    // Signup page widgets
    QWidget* signupPage;
    QLineEdit* signupUsername;
    QLineEdit* signupPassword;
    QLineEdit* signupConfirmPassword;
    QPushButton* signupButton;
    QPushButton* goToLoginButton;
    QLabel* signupStatusLabel;

   
    void setupLoginPage();
    void setupSignupPage();
   
};

#endif // MAINWINDOW_H

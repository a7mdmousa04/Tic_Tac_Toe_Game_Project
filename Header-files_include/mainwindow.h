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
    QComboBox* difficultyComboBox;
    QPushButton* difficultyConfirmButton;
    void handleDifficultyChanged();

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

     // Menu page widgets
    QWidget* menuPage;
    QPushButton* playButton;
    QPushButton* historyButton;
    QPushButton* logoutButton;
    QLabel* welcomeLabel;

    // Game mode page widgets
    QWidget* gameModePage;
    QPushButton* twoPlayerButton;
    QPushButton* aiButton;
    QPushButton* backToMenuFromGameModeButton;

    // Game page widgets
    QWidget* gamePage;
    QLabel* gameStatusLabel;
    QPushButton* backToMenuFromGameButton;
    QPushButton* boardButtons[3][3];
    QGridLayout* boardLayout;

    // History page widgets
    QWidget* historyPage;
    QListWidget* gamesList;
    QPushButton* loadGameButton;
    QPushButton* backToMenuFromHistoryButton;
    QLabel* replayStatusLabel;
    QSlider* replaySlider;
    QPushButton* previousMoveButton;
    QPushButton* nextMoveButton;
    QPushButton* boardReplayButtons[3][3];
    QGridLayout* replayBoardLayout;
   
    void setupLoginPage();
    void setupSignupPage();
    void setupMenuPage();
    void setupGameModePage();
    void setupGamePage();
    void setupHistoryPage();
    void updateBoardButtons();
    void updateReplayBoardButtons();
    void loadGameHistory();
};

#endif // MAINWINDOW_H

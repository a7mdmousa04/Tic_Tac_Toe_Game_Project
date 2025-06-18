#include "mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDateTime>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QRadioButton>
#include <QButtonGroup>

MainWindow::MainWindow(UserAuth* auth, QWidget* parent)
    : QMainWindow(parent), userAuth(auth), playerIsX(true)
{
    // Set window properties
    setWindowTitle("Advanced Tic Tac Toe");
    resize(600, 700);

    // Creative background with gaming theme
    setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #1a1a2e, stop:0.3 #16213e, stop:0.6 #0f3460, stop:1 #533483);"
        "}"
        "QWidget {"
        "background: transparent;"
        "}"
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #ff6b6b, stop:0.5 #ee5a24, stop:1 #ff9ff3);"
        "border: 3px solid #ffffff;"
        "border-radius: 20px;"
        "color: #ffffff;"
        "font-weight: bold;"
        "font-size: 14px;"
        "padding: 12px;"
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.7);"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #feca57, stop:0.5 #ff9ff3, stop:1 #54a0ff);"
        "transform: scale(1.05);"
        "border: 3px solid #f1c40f;"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #a29bfe, stop:1 #fd79a8);"
        "}"
        "QLabel {"
        "color: #ffffff;"
        "font-weight: bold;"
        "text-shadow: 3px 3px 6px rgba(0,0,0,0.8);"
        "}"
        "QLineEdit {"
        "background: rgba(255,255,255,0.95);"
        "border: 3px solid #74b9ff;"
        "border-radius: 15px;"
        "padding: 10px;"
        "font-size: 14px;"
        "color: #2d3436;"
        "}"
        "QListWidget {"
        "background: rgba(255,255,255,0.95);"
        "border: 3px solid #74b9ff;"
        "border-radius: 15px;"
        "color: #2d3436;"
        "}"
        "QComboBox {"
        "background: rgba(255,255,255,0.95);"
        "border: 3px solid #74b9ff;"
        "border-radius: 15px;"
        "padding: 10px;"
        "color: #2d3436;"
        "}"
        "QSlider::groove:horizontal {"
        "border: 2px solid #74b9ff;"
        "height: 10px;"
        "background: rgba(255,255,255,0.8);"
        "border-radius: 6px;"
        "}"
        "QSlider::handle:horizontal {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #ff6b6b, stop:1 #feca57);"
        "border: 3px solid #ffffff;"
        "width: 20px;"
        "border-radius: 12px;"
        "}"
        "QRadioButton {"
        "color: #ffffff;"
        "font-size: 16px;"
        "font-weight: bold;"
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.7);"
        "}"
    );

    // Create game logic
    gameLogic = new GameLogic(this);

    // Create stacked widget for different pages
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Setup all pages
    setupLoginPage();
    setupSignupPage();
    setupMenuPage();
    setupGameModePage();
    setupGamePage();
    setupHistoryPage();

    // Add pages to stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(signupPage);
    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gameModePage);
    stackedWidget->addWidget(gamePage);
    stackedWidget->addWidget(historyPage);

    // Connect signals from game logic
    connect(gameLogic, &GameLogic::boardChanged, this, &MainWindow::updateBoard);
    connect(gameLogic, &GameLogic::gameEnded, this, &MainWindow::handleGameEnd);

    // Show login page initially
    stackedWidget->setCurrentWidget(loginPage);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupGamePage()
{
    gamePage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gamePage);

    // Creative title with gaming theme
    QLabel* titleLabel = new QLabel("🎮 TIC TAC TOE 🎮");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    gameStatusLabel = new QLabel("Make Your Move!");
    gameStatusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = gameStatusLabel->font();
    statusFont.setPointSize(16);
    gameStatusLabel->setFont(statusFont);
    gameStatusLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    // Board layout with creative styling
    QWidget* boardWidget = new QWidget();
    boardWidget->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 rgba(116, 185, 255, 0.3), stop:1 rgba(241, 196, 15, 0.3));"
        "border: 4px solid #ffffff;"
        "border-radius: 20px;"
        "}"
    );
    boardLayout = new QGridLayout(boardWidget);
    boardLayout->setSpacing(8);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            boardButtons[row][col] = new QPushButton("");
            boardButtons[row][col]->setFixedSize(100, 100);
            QFont buttonFont = boardButtons[row][col]->font();
            buttonFont.setPointSize(32);
            buttonFont.setBold(true);
            boardButtons[row][col]->setFont(buttonFont);

            // Special styling for game board buttons
            boardButtons[row][col]->setStyleSheet(
                "QPushButton {"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #ffffff, stop:0.5 #f8f9fa, stop:1 #e9ecef);"
                "border: 4px solid #495057;"
                "border-radius: 15px;"
                "font-size: 32px;"
                "font-weight: bold;"
                "color: #495057;"
                "}"
                "QPushButton:hover {"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #74b9ff, stop:1 #0984e3);"
                "color: #ffffff;"
                "border: 4px solid #f1c40f;"
                "transform: scale(1.1);"
                "}"
            );

            // Store row and col as properties for later identification
            boardButtons[row][col]->setProperty("row", row);
            boardButtons[row][col]->setProperty("col", col);

            // Connect button to click handler
            connect(boardButtons[row][col], &QPushButton::clicked,
                this, &MainWindow::handleCellClicked);

            boardLayout->addWidget(boardButtons[row][col], row, col);
        }
    }

    backToMenuFromGameButton = new QPushButton("Return");
    backToMenuFromGameButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #636e72, stop:1 #2d3436);"
        "color: #ffffff;"
        "font-size: 16px;"
        "padding: 15px;"
        "}"
    );

    layout->addWidget(titleLabel);
    layout->addSpacing(10);
    layout->addWidget(gameStatusLabel);
    layout->addSpacing(20);
    layout->addWidget(boardWidget, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(backToMenuFromGameButton);

    // Connect back button
    connect(backToMenuFromGameButton, &QPushButton::clicked,
        this, &MainWindow::showMenuPage);
}

void MainWindow::setupLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(loginPage);

    QLabel* titleLabel = new QLabel("🎮 TIC TAC TOE 🎮");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    QLabel* subtitleLabel = new QLabel(" Enter the Gaming Arena");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(18);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    QFormLayout* formLayout = new QFormLayout();

    loginUsername = new QLineEdit();
    loginUsername->setPlaceholderText("Enter your warrior name");
    formLayout->addRow(" Username:", loginUsername);

    loginPassword = new QLineEdit();
    loginPassword->setPlaceholderText("Enter your secret code");
    loginPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow(" Password:", loginPassword);

    loginButton = new QPushButton("SIGN IN");
    loginButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #00b894, stop:1 #00a085);"
        "font-size: 18px;"
        "padding: 15px;"
        "}"
    );

    goToSignupButton = new QPushButton("📝 Don't have an account? sign up");
    goToSignupButton->setStyleSheet(
        "QPushButton {"
        "background: transparent;"
        "border: 2px solid #ffffff;"
        "color: #ffffff;"
        "text-decoration: underline;"
        "}"
    );

    loginStatusLabel = new QLabel("");
    loginStatusLabel->setStyleSheet("color: #ff7675; font-weight: bold; font-size: 14px;");

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(30);
    layout->addLayout(formLayout);
    layout->addSpacing(20);
    layout->addWidget(loginButton);
    layout->addWidget(goToSignupButton);
    layout->addWidget(loginStatusLabel);
    layout->addStretch();

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(goToSignupButton, &QPushButton::clicked, this, &MainWindow::showSignupPage);
}

void MainWindow::setupSignupPage()
{
    signupPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(signupPage);

    QLabel* titleLabel = new QLabel("🎮 TIC TAC TOE 🎮");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    QLabel* subtitleLabel = new QLabel("Let's start");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(18);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    QFormLayout* formLayout = new QFormLayout();

    signupUsername = new QLineEdit();
    signupUsername->setPlaceholderText("Username");
    formLayout->addRow("Username:", signupUsername);

    signupPassword = new QLineEdit();
    signupPassword->setPlaceholderText("Password");
    signupPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Password:", signupPassword);

    signupConfirmPassword = new QLineEdit();
    signupConfirmPassword->setPlaceholderText("Confirm Password");
    signupConfirmPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Confirm Password:", signupConfirmPassword);

    signupButton = new QPushButton("SIGN UP");
    signupButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #74b9ff, stop:1 #0984e3);"
        "font-size: 18px;"
        "padding: 15px;"
        "}"
    );

    goToLoginButton = new QPushButton("Already have an account? Sign in");
    goToLoginButton->setStyleSheet(
        "QPushButton {"
        "background: transparent;"
        "border: 2px solid #ffffff;"
        "color: #ffffff;"
        "text-decoration: underline;"
        "}"
    );

    signupStatusLabel = new QLabel("");
    signupStatusLabel->setStyleSheet("color: #ff7675; font-weight: bold; font-size: 14px;");

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(30);
    layout->addLayout(formLayout);
    layout->addSpacing(20);
    layout->addWidget(signupButton);
    layout->addWidget(goToLoginButton);
    layout->addWidget(signupStatusLabel);
    layout->addStretch();

    // Connect signals
    connect(signupButton, &QPushButton::clicked, this, &MainWindow::handleSignup);
    connect(goToLoginButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);
}

void MainWindow::setupMenuPage()
{
    menuPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(menuPage);

    QLabel* titleLabel = new QLabel("🎮 TIC TAC TOE 🎮");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    welcomeLabel = new QLabel("🌟 Welcome" + userAuth->getCurrentUser() + "! 🌟");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(18);
    welcomeLabel->setFont(welcomeFont);
    welcomeLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    playButton = new QPushButton("Start");
    playButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #00b894, stop:1 #00a085);"
        "font-size: 20px;"
        "padding: 20px;"
        "}"
    );
    playButton->setMinimumHeight(80);

    historyButton = new QPushButton("History");
    historyButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #74b9ff, stop:1 #0984e3);"
        "font-size: 20px;"
        "padding: 20px;"
        "}"
    );
    historyButton->setMinimumHeight(80);

    logoutButton = new QPushButton("EXIT");
    logoutButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e17055, stop:1 #d63031);"
        "font-size: 16px;"
        "padding: 15px;"
        "}"
    );

    layout->addWidget(titleLabel);
    layout->addWidget(welcomeLabel);
    layout->addSpacing(40);
    layout->addWidget(playButton);
    layout->addSpacing(20);
    layout->addWidget(historyButton);
    layout->addSpacing(40);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Connect signals
    connect(playButton, &QPushButton::clicked, this, &MainWindow::showGameModePage);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::showHistoryPage);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);
}

void MainWindow::setupGameModePage() {
    gameModePage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gameModePage);

    QLabel* titleLabel = new QLabel("CHOOSE MODE");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    twoPlayerButton = new QPushButton("Player VS Player");
    twoPlayerButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #fdcb6e, stop:1 #e17055);"
        "font-size: 18px;"
        "padding: 18px;"
        "}"
    );
    twoPlayerButton->setMinimumHeight(70);

    aiButton = new QPushButton("Player VS AI");
    aiButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #a29bfe, stop:1 #6c5ce7);"
        "font-size: 18px;"
        "padding: 18px;"
        "}"
    );
    aiButton->setMinimumHeight(70);

    // Add difficulty selection
    QLabel* difficultyLabel = new QLabel("AI Level:");
    difficultyLabel->setAlignment(Qt::AlignCenter);
    QFont difficultyFont = difficultyLabel->font();
    difficultyFont.setPointSize(16);
    difficultyLabel->setFont(difficultyFont);
    difficultyLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    difficultyComboBox = new QComboBox();
    difficultyComboBox->addItem("Easy", static_cast<int>(AIDifficulty::Easy));
    difficultyComboBox->addItem("Medium", static_cast<int>(AIDifficulty::Medium));
    difficultyComboBox->addItem("Hard", static_cast<int>(AIDifficulty::Hard));
    difficultyComboBox->addItem("Unbeatable", static_cast<int>(AIDifficulty::Unbeatable));
    difficultyComboBox->setCurrentIndex(1); // Medium by default
    difficultyComboBox->setStyleSheet(
                "QComboBox {"
                "    border: 2px solid rgb(255, 255, 255);"
                "    border-radius: 15px;"
                "    padding: 8px 18px 8px 12px;"
                "    min-width: 120px;"
                "    min-height: 25px;"
                "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 rgb(52, 152, 219),"
                "                               stop: 0.5 rgb(41, 128, 185),"
                "                               stop: 1.0 rgb(31, 97, 141));"
                "    color: white;"
                "    font-weight: bold;"
                "    font-size: 14px;"
                "}"
                "QComboBox:hover {"
                "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 rgb(93, 173, 226),"
                "                               stop: 0.5 rgb(52, 152, 219),"
                "                               stop: 1.0 rgb(41, 128, 185));"
                "    border-color: rgb(26, 188, 156);"
                "}"
                "QComboBox:pressed {"
                "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 rgb(41, 128, 185),"
                "                               stop: 0.5 rgb(31, 97, 141),"
                "                               stop: 1.0 rgb(21, 67, 96));"
                "}"
                "QComboBox::drop-down {"
                "    subcontrol-origin: padding;"
                "    subcontrol-position: top right;"
                "    width: 25px;"
                "    border-left-width: 2px;"
                "    border-left-color: rgb(52, 73, 94);"
                "    border-left-style: solid;"
                "    border-top-right-radius: 15px;"
                "    border-bottom-right-radius: 15px;"
                "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 rgb(52, 73, 94),"
                "                               stop: 1.0 rgb(44, 62, 80));"
                "}"
                "QComboBox::down-arrow {"
                "    width: 12px;"
                "    height: 12px;"
                "    background: white;"
                "    border: 2px solid white;"
                "    border-radius: 2px;"
                "}"
                "QComboBox QAbstractItemView {"
                "    border: 2px solid rgb(44, 62, 80);"
                "    border-radius: 10px;"
                "    background-color: grey;"
                "    selection-background-color: rgb(52, 152, 219);"
                "    selection-color: white;"
                "    outline: none;"
                "}"
                "QComboBox QAbstractItemView::item {"
                "    padding: 8px;"
                "    border-bottom: 1px solid rgb(236, 240, 241);"
                "}"
                "QComboBox QAbstractItemView::item:hover {"
                "    background-color: rgb(235, 243, 253);"
                "    color: rgb(44, 62, 80);"
                "}"
                "QComboBox QAbstractItemView::item:selected {"
                "    background-color: rgb(52, 152, 219);"
                "    color: white;"
                "}"
        );
    difficultyConfirmButton = new QPushButton("Set AI Level");
    difficultyConfirmButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #00b894, stop:1 #00a085);"
        "font-size: 14px;"
        "padding: 12px;"
        "}"
    );

    backToMenuFromGameModeButton = new QPushButton("Back");
    backToMenuFromGameModeButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #636e72, stop:1 #2d3436);"
        "font-size: 14px;"
        "padding: 12px;"
        "}"
    );

    layout->addWidget(titleLabel);
    layout->addSpacing(25);
    layout->addSpacing(25);
    layout->addWidget(twoPlayerButton);
    layout->addSpacing(15);
    layout->addWidget(aiButton);
    layout->addSpacing(25);
    layout->addWidget(difficultyLabel);
    layout->addWidget(difficultyComboBox);
    layout->addWidget(difficultyConfirmButton);
    layout->addSpacing(25);
    layout->addWidget(backToMenuFromGameModeButton);
    layout->addStretch();

    // Connect signals
    connect(twoPlayerButton, &QPushButton::clicked, this, &MainWindow::startTwoPlayerGame);
    connect(aiButton, &QPushButton::clicked, this, &MainWindow::startAIGame);
    connect(difficultyConfirmButton, &QPushButton::clicked, this, &MainWindow::handleDifficultyChanged);
    connect(backToMenuFromGameModeButton, &QPushButton::clicked, this, &MainWindow::showMenuPage);
}

void MainWindow::handleDifficultyChanged() {
    AIDifficulty difficulty = static_cast<AIDifficulty>(difficultyComboBox->currentData().toInt());
    gameLogic->setDifficulty(difficulty);

    QString difficultyName = difficultyComboBox->currentText();
    QMessageBox::information(this, "AI Level Set", "AI Level: " + difficultyName);
}

void MainWindow::setupHistoryPage()
{
    historyPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(historyPage);

    QLabel* titleLabel = new QLabel("HISTORY");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #f1c40f; text-shadow: 4px 4px 8px rgba(0,0,0,0.9);");

    // Game list
    gamesList = new QListWidget();
    gamesList->setAlternatingRowColors(true);

    // Buttons for managing history
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    loadGameButton = new QPushButton("Replay");
    loadGameButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #74b9ff, stop:1 #0984e3);"
        "font-size: 16px;"
        "padding: 12px;"
        "}"
    );
    backToMenuFromHistoryButton = new QPushButton("Back");
    backToMenuFromHistoryButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #636e72, stop:1 #2d3436);"
        "font-size: 16px;"
        "padding: 12px;"
        "}"
    );

    buttonsLayout->addWidget(loadGameButton);
    buttonsLayout->addWidget(backToMenuFromHistoryButton);

    // Replay controls
    QHBoxLayout* replayControlsLayout = new QHBoxLayout();
    previousMoveButton = new QPushButton("⏪");
    nextMoveButton = new QPushButton("⏩");
    replaySlider = new QSlider(Qt::Horizontal);
    replaySlider->setEnabled(false);

    replayControlsLayout->addWidget(previousMoveButton);
    replayControlsLayout->addWidget(replaySlider);
    replayControlsLayout->addWidget(nextMoveButton);

    // Replay status
    replayStatusLabel = new QLabel("Select the game you want to replay");
    replayStatusLabel->setAlignment(Qt::AlignCenter);
    replayStatusLabel->setStyleSheet("color: #00ff88; text-shadow: 3px 3px 6px rgba(0,0,0,0.8);");

    // Replay board
    QWidget* replayBoardWidget = new QWidget();
    replayBoardWidget->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 rgba(116, 185, 255, 0.2), stop:1 rgba(241, 196, 15, 0.2));"
        "border: 3px solid #ffffff;"
        "border-radius: 15px;"
        "}"
    );
    replayBoardLayout = new QGridLayout(replayBoardWidget);
    replayBoardLayout->setSpacing(5);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            boardReplayButtons[row][col] = new QPushButton("");
            boardReplayButtons[row][col]->setFixedSize(70, 70);
            QFont buttonFont = boardReplayButtons[row][col]->font();
            buttonFont.setPointSize(20);
            buttonFont.setBold(true);
            boardReplayButtons[row][col]->setFont(buttonFont);
            boardReplayButtons[row][col]->setEnabled(false);

            // Special styling for replay board buttons
            boardReplayButtons[row][col]->setStyleSheet(
                "QPushButton {"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #ffffff, stop:1 #f0f0f0);"
                "border: 3px solid #495057;"
                "border-radius: 10px;"
                "font-size: 20px;"
                "font-weight: bold;"
                "}"
            );

            replayBoardLayout->addWidget(boardReplayButtons[row][col], row, col);
        }
    }

    layout->addWidget(titleLabel);
    layout->addSpacing(15);
    layout->addWidget(gamesList);
    layout->addLayout(buttonsLayout);
    layout->addSpacing(25);
    layout->addWidget(replayStatusLabel);
    layout->addLayout(replayControlsLayout);
    layout->addWidget(replayBoardWidget, 0, Qt::AlignCenter);

    // Connect signals
    connect(loadGameButton, &QPushButton::clicked, this, &MainWindow::loadSelectedGame);
    connect(backToMenuFromHistoryButton, &QPushButton::clicked, this, &MainWindow::showMenuPage);
    connect(replaySlider, &QSlider::valueChanged, this, &MainWindow::updateReplay);
    connect(previousMoveButton, &QPushButton::clicked, this, &MainWindow::playPreviousMove);
    connect(nextMoveButton, &QPushButton::clicked, this, &MainWindow::playNextMove);
}

void MainWindow::handleCellClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = button->property("row").toInt();
        int col = button->property("col").toInt();

        gameLogic->makeMove(row, col);
    }
}

void MainWindow::updateBoard()
{
    // Update the board with the current game state
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Player cell = gameLogic->getCell(row, col);
            QPushButton* button = boardButtons[row][col];

            if (cell == Player::X) {
                button->setText("❌");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #ff7675, stop:1 #fd79a8);"
                    "border: 4px solid #e84393;"
                    "border-radius: 15px;"
                    "font-size: 28px;"
                    "color: #ffffff;"
                    "text-shadow: 2px 2px 4px rgba(0,0,0,0.7);"
                    "}"
                );
            }
            else if (cell == Player::O) {
                button->setText("⭕");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #74b9ff, stop:1 #0984e3);"
                    "border: 4px solid #00b894;"
                    "border-radius: 15px;"
                    "font-size: 28px;"
                    "color: #ffffff;"
                    "text-shadow: 2px 2px 4px rgba(0,0,0,0.7);"
                    "}"
                );
            }
            else {
                button->setText("");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #ffffff, stop:0.5 #f8f9fa, stop:1 #e9ecef);"
                    "border: 4px solid #495057;"
                    "border-radius: 15px;"
                    "font-size: 32px;"
                    "font-weight: bold;"
                    "color: #495057;"
                    "}"
                    "QPushButton:hover {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #74b9ff, stop:1 #0984e3);"
                    "color: #ffffff;"
                    "border: 4px solid #f1c40f;"
                    "transform: scale(1.1);"
                    "}"
                );
            }
        }
    }

    // Update game status label with creative messages
    if (!gameLogic->isGameOver()) {
        Player currentPlayer = gameLogic->getCurrentPlayer();
        if (currentPlayer == Player::X) {
            gameStatusLabel->setText("X's Turn");
        }
        else {
            gameStatusLabel->setText("O's Turn");
        }
    }
}

void MainWindow::updateBoardButtons()
{
    updateBoard(); // Use the same logic
}

void MainWindow::updateReplayBoardButtons()
{
    // Update the replay board with the current game state
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Player cell = gameLogic->getCell(row, col);
            QPushButton* button = boardReplayButtons[row][col];

            if (cell == Player::X) {
                button->setText("❌");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #ff7675, stop:1 #fd79a8);"
                    "border: 3px solid #e84393;"
                    "border-radius: 10px;"
                    "font-size: 18px;"
                    "color: #ffffff;"
                    "}"
                );
            }
            else if (cell == Player::O) {
                button->setText("⭕");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #74b9ff, stop:1 #0984e3);"
                    "border: 3px solid #00b894;"
                    "border-radius: 10px;"
                    "font-size: 18px;"
                    "color: #ffffff;"
                    "}"
                );
            }
            else {
                button->setText("");
                button->setStyleSheet(
                    "QPushButton {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                    "stop:0 #ffffff, stop:1 #f0f0f0);"
                    "border: 3px solid #495057;"
                    "border-radius: 10px;"
                    "font-size: 20px;"
                    "font-weight: bold;"
                    "}"
                );
            }
        }
    }
}

void MainWindow::handleGameEnd(Player winner)
{
    QString resultText;

    if (winner == Player::None) {
        resultText = "🤝 TIE ";
    }
    else {
        QString winnerText = (winner == Player::X) ? "❌" : "⭕";
        if ((winner == Player::X && playerIsX) || (winner == Player::O && !playerIsX)) {
            resultText = "🏆 player "+ winnerText +" Won" ;
        }
        else {
            resultText = "🏆 player "+ winnerText +" Won" ;
        }
    }

    gameStatusLabel->setText(resultText);

    // Save game to history
    if (userAuth->isLoggedIn()) {
        QJsonObject gameData = gameLogic->getGameAsJson();
        userAuth->saveGameToHistory(gameData);
    }
}

void MainWindow::showLoginPage()
{
    loginUsername->clear();
    loginPassword->clear();
    loginStatusLabel->clear();
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::showSignupPage()
{
    signupUsername->clear();
    signupPassword->clear();
    signupConfirmPassword->clear();
    signupStatusLabel->clear();
    stackedWidget->setCurrentWidget(signupPage);
}

void MainWindow::showMenuPage()
{
    if (userAuth->isLoggedIn()) {
        welcomeLabel->setText("🌟 Welcome back, " + userAuth->getCurrentUser() + "! 🌟");
        stackedWidget->setCurrentWidget(menuPage);
    }
    else {
        showLoginPage();
    }
}

void MainWindow::showGamePage()
{
    updateBoardButtons();
    stackedWidget->setCurrentWidget(gamePage);
}

void MainWindow::showGameModePage()
{
    stackedWidget->setCurrentWidget(gameModePage);
}

void MainWindow::showHistoryPage()
{
    loadGameHistory();
    stackedWidget->setCurrentWidget(historyPage);
}

void MainWindow::handleLogin()
{
    QString username = loginUsername->text();
    QString password = loginPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        loginStatusLabel->setText("Complete all fields");
        return;
    }

    if (userAuth->signIn(username, password)) {
        showMenuPage();
    }
    else {
        loginStatusLabel->setText("Invalid username or password!");
    }
}

void MainWindow::handleSignup() {
    QString username = signupUsername->text();
    QString password = signupPassword->text();
    QString confirmPassword = signupConfirmPassword->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        signupStatusLabel->setText("Complete all fields");
        return;
    }

    if (password != confirmPassword) {
        signupStatusLabel->setText("Passwords don't match");
        return;
    }

    // Check username format
    for (const QChar& ch : username) {
        if (!ch.isLetterOrNumber() && ch != '_') {
            signupStatusLabel->setText("Username contains letters, numbers, underscores only!");
            return;
        }
    }

    // Check password length
    if (password.length() < 8) {
        signupStatusLabel->setText("Password needs 8 or more characters!");
        return;
    }

    // Check for uppercase letter
    bool hasUppercase = false;
    for (const QChar& ch : password) {
        if (ch.isUpper()) {
            hasUppercase = true;
            break;
        }
    }
    if (!hasUppercase) {
        signupStatusLabel->setText("Password needs at least an uppercase character");
        return;
    }

    // Check for special character
    QString specialChars = "!@#$%^&*()_-+=<>?/[]{}|\\";
    bool hasSpecial = false;
    for (const QChar& ch : password) {
        if (specialChars.contains(ch)) {
            hasSpecial = true;
            break;
        }
    }
    if (!hasSpecial) {
        signupStatusLabel->setText("Password needs at least one special character!");
        return;
    }

    if (userAuth->signUp(username, password)) {
        QMessageBox::information(this, "User Created", "Welcome, Please enter to begin the game");
        showLoginPage();
    }
    else {
        signupStatusLabel->setText("Username is already taken!");
    }
}

void MainWindow::handleLogout()
{
    userAuth->signOut();
    showLoginPage();
}

void MainWindow::startTwoPlayerGame()
{
    // Check which symbol the player chose
    playerIsX = true; // 0 for X, 1 for O

    gameLogic->newGame(false); // Start game with 2 players
    showGamePage();
}

void MainWindow::startAIGame() {
    AIDifficulty difficulty = static_cast<AIDifficulty>(difficultyComboBox->currentData().toInt());
    gameLogic->setDifficulty(difficulty);

    // Check which symbol the player chose
    playerIsX = true; // 0 for X, 1 for O

    gameLogic->newGame(true); // Start game with AI

    // FIXED: Only make AI move if player chose O and it's X's turn
    if (!playerIsX && gameLogic->getCurrentPlayer() == Player::X) {
        QTimer::singleShot(800, [this]() {
            gameLogic->aiMove();
            });
    }

    showGamePage();
}

void MainWindow::loadGameHistory()
{
    gamesList->clear();

    if (!userAuth->isLoggedIn()) {
        return;
    }

    QJsonArray history = userAuth->getGameHistory();

    for (int i = 0; i < history.size(); ++i) {
        QJsonObject gameData = history[i].toObject();
        QString date = gameData["date"].toString();
        QString result = gameData["result"].toString();
        QString vsAI = gameData["vsAI"].toBool() ? "player vs AI" : "player vs Player";

        QListWidgetItem* item = new QListWidgetItem(QString(" %1 - %2 (%3)").arg(date).arg(result).arg(vsAI));
        item->setData(Qt::UserRole, i); // Store the game index
        gamesList->addItem(item);
    }
}

void MainWindow::loadSelectedGame()
{
    QListWidgetItem* selectedItem = gamesList->currentItem();
    if (!selectedItem) {
        return;
    }

    int gameIndex = selectedItem->data(Qt::UserRole).toInt();
    QJsonArray history = userAuth->getGameHistory();

    if (gameIndex >= 0 && gameIndex < history.size()) {
        QJsonObject gameData = history[gameIndex].toObject();

        // Load game into game logic
        gameLogic->loadFromJson(gameData);

        // Setup replay controls
        QJsonArray movesArray = gameData["moves"].toArray();
        replaySlider->setMinimum(0);
        replaySlider->setMaximum(movesArray.size());
        replaySlider->setValue(movesArray.size());
        replaySlider->setEnabled(true);

        // Update status
        QString vsAI = gameData["vsAI"].toBool() ? "player vs AI" : "player vs Player";
        replayStatusLabel->setText(QString("Reliving game from %1 (%2)").arg(gameData["date"].toString()).arg(vsAI));

        // Update board
        updateReplayBoardButtons();
    }
}

void MainWindow::updateReplay(int value)
{
    gameLogic->replayMove(value);
    updateReplayBoardButtons();

    // Update status with epic messages
    QVector<Move> moves = gameLogic->getMoves();
    if (value == 0) {
        replayStatusLabel->setText("Game starts...");
    }
    else if (value == moves.size()) {
        QString endText = "Game ends";
        replayStatusLabel->setText(endText);
    }
    else {
        Player lastMovePlayer = (value % 2 == 1) ? Player::X : Player::O;
        QString playerText = (lastMovePlayer == Player::X) ? "❌" : "⭕";
        replayStatusLabel->setText(" Move " + QString::number(value) + " - " + playerText + "plays");
    }
}

void MainWindow::playNextMove()
{
    if (replaySlider->isEnabled()) {
        int currentValue = replaySlider->value();
        int maxValue = replaySlider->maximum();

        if (currentValue < maxValue) {
            replaySlider->setValue(currentValue + 1);
        }
    }
}

void MainWindow::playPreviousMove()
{
    if (replaySlider->isEnabled()) {
        int currentValue = replaySlider->value();

        if (currentValue > 0) {
            replaySlider->setValue(currentValue - 1);
        }
    }
}

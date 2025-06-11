// mainwindow.cpp - Main Window Implementation
#include "mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(UserAuth* auth, QWidget *parent)
    : QMainWindow(parent), userAuth(auth)
{
    // Set window properties
    setWindowTitle("Advanced Tic Tac Toe");
    resize(500, 600);

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

void MainWindow::setupLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(loginPage);

    QLabel* titleLabel = new QLabel("Advanced Tic Tac Toe");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel* subtitleLabel = new QLabel("Login to Play");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(14);
    subtitleLabel->setFont(subtitleFont);

    QFormLayout* formLayout = new QFormLayout();

    loginUsername = new QLineEdit();
    loginUsername->setPlaceholderText("Enter your username");
    formLayout->addRow("Username:", loginUsername);

    loginPassword = new QLineEdit();
    loginPassword->setPlaceholderText("Enter your password");
    loginPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Password:", loginPassword);

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("background-color: #4CAF50; color: white;");

    goToSignupButton = new QPushButton("Don't have an account? Sign up");
    goToSignupButton->setStyleSheet("background-color: transparent; text-decoration: underline; border: none;");

    loginStatusLabel = new QLabel("");
    loginStatusLabel->setStyleSheet("color: red;");

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addLayout(formLayout);
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

    QLabel* titleLabel = new QLabel("Advanced Tic Tac Toe");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel* subtitleLabel = new QLabel("Create an Account");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(14);
    subtitleLabel->setFont(subtitleFont);

    QFormLayout* formLayout = new QFormLayout();

    signupUsername = new QLineEdit();
    signupUsername->setPlaceholderText("Choose a username");
    formLayout->addRow("Username:", signupUsername);

    signupPassword = new QLineEdit();
    signupPassword->setPlaceholderText("Choose a password");
    signupPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Password:", signupPassword);

    signupConfirmPassword = new QLineEdit();
    signupConfirmPassword->setPlaceholderText("Confirm your password");
    signupConfirmPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Confirm Password:", signupConfirmPassword);

    signupButton = new QPushButton("Sign Up");
    signupButton->setStyleSheet("background-color: #2196F3; color: white;");

    goToLoginButton = new QPushButton("Already have an account? Login");
    goToLoginButton->setStyleSheet("background-color: transparent; text-decoration: underline; border: none;");

    signupStatusLabel = new QLabel("");
    signupStatusLabel->setStyleSheet("color: red;");

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addLayout(formLayout);
    layout->addWidget(signupButton);
    layout->addWidget(goToLoginButton);
    layout->addWidget(signupStatusLabel);
    layout->addStretch();

    // Connect signals
    // mainwindow.cpp (continued)
    connect(signupButton, &QPushButton::clicked, this, &MainWindow::handleSignup);
    connect(goToLoginButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);
}

void MainWindow::setupMenuPage()
{
    menuPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(menuPage);

    QLabel* titleLabel = new QLabel("Advanced Tic Tac Toe");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(14);
    welcomeLabel->setFont(welcomeFont);

    playButton = new QPushButton("Play");
    playButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px;");
    playButton->setMinimumHeight(50);

    historyButton = new QPushButton("Game History");
    historyButton->setStyleSheet("background-color: #2196F3; color: white; font-size: 16px;");
    historyButton->setMinimumHeight(50);

    logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("background-color: #f44336; color: white; font-size: 14px;");

    layout->addWidget(titleLabel);
    layout->addWidget(welcomeLabel);
    layout->addSpacing(20);
    layout->addWidget(playButton);
    layout->addSpacing(10);
    layout->addWidget(historyButton);
    layout->addSpacing(20);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Connect signals
    connect(playButton, &QPushButton::clicked, this, &MainWindow::showGameModePage);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::showHistoryPage);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);
}

void MainWindow::setupGameModePage()
{
    gameModePage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gameModePage);

    QLabel* titleLabel = new QLabel("Choose Game Mode");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    twoPlayerButton = new QPushButton("Player vs Player");
    twoPlayerButton->setStyleSheet("background-color: #FF9800; color: white; font-size: 16px;");
    twoPlayerButton->setMinimumHeight(60);

    aiButton = new QPushButton("Player vs AI");
    aiButton->setStyleSheet("background-color: #9C27B0; color: white; font-size: 16px;");
    aiButton->setMinimumHeight(60);

    // Add difficulty selection
    QLabel* difficultyLabel = new QLabel("AI Difficulty:");
    difficultyLabel->setAlignment(Qt::AlignCenter);
    QFont difficultyFont = difficultyLabel->font();
    difficultyFont.setPointSize(14);
    difficultyLabel->setFont(difficultyFont);

    difficultyComboBox = new QComboBox();
    difficultyComboBox->addItem("Easy", static_cast<int>(AIDifficulty::Easy));
    difficultyComboBox->addItem("Medium", static_cast<int>(AIDifficulty::Medium));
    difficultyComboBox->addItem("Hard", static_cast<int>(AIDifficulty::Hard));
    difficultyComboBox->addItem("Unbeatable", static_cast<int>(AIDifficulty::Unbeatable));
    difficultyComboBox->setCurrentIndex(1); // Medium by default

    difficultyConfirmButton = new QPushButton("Confirm Difficulty");
    difficultyConfirmButton->setStyleSheet("background-color: #4CAF50; color: white;");

    backToMenuFromGameModeButton = new QPushButton("Back to Menu");
    backToMenuFromGameModeButton->setStyleSheet("color: #424242;");

    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(twoPlayerButton);
    layout->addSpacing(10);
    layout->addWidget(aiButton);
    layout->addSpacing(20);
    layout->addWidget(difficultyLabel);
    layout->addWidget(difficultyComboBox);
    layout->addWidget(difficultyConfirmButton);
    layout->addSpacing(20);
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
    QMessageBox::information(this, "Difficulty Set", "AI difficulty set to: " + difficultyName);
}

void MainWindow::setupGamePage()
{
    gamePage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gamePage);

    // Game status label
    gameStatusLabel = new QLabel("Player X's Turn");
    gameStatusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = gameStatusLabel->font();
    statusFont.setPointSize(16);
    gameStatusLabel->setFont(statusFont);

    // Create board layout and buttons
    QWidget* boardWidget = new QWidget();
    boardLayout = new QGridLayout(boardWidget);
    boardLayout->setSpacing(5);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            boardButtons[row][col] = new QPushButton("");
            boardButtons[row][col]->setFixedSize(80, 80);
            QFont buttonFont = boardButtons[row][col]->font();
            buttonFont.setPointSize(24);
            buttonFont.setBold(true);
            boardButtons[row][col]->setFont(buttonFont);
            boardButtons[row][col]->setProperty("row", row);
            boardButtons[row][col]->setProperty("col", col);

            connect(boardButtons[row][col], &QPushButton::clicked, this, &MainWindow::handleCellClicked);

            boardLayout->addWidget(boardButtons[row][col], row, col);
        }
    }

    // Back to menu button
    backToMenuFromGameButton = new QPushButton("Back to Menu");
    backToMenuFromGameButton->setStyleSheet("color: #424242;");

    layout->addWidget(gameStatusLabel);
    layout->addSpacing(10);
    layout->addWidget(boardWidget, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(backToMenuFromGameButton);

    // Connect signals
    connect(backToMenuFromGameButton, &QPushButton::clicked, this, &MainWindow::showMenuPage);
}

void MainWindow::setupHistoryPage()
{
    historyPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(historyPage);

    QLabel* titleLabel = new QLabel("Game History");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Game list
    gamesList = new QListWidget();
    gamesList->setAlternatingRowColors(true);

    // Buttons for managing history
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    loadGameButton = new QPushButton("Replay Selected Game");
    loadGameButton->setStyleSheet("background-color: #2196F3; color: white;");
    backToMenuFromHistoryButton = new QPushButton("Back to Menu");
    backToMenuFromHistoryButton->setStyleSheet("color: #424242;");

    buttonsLayout->addWidget(loadGameButton);
    buttonsLayout->addWidget(backToMenuFromHistoryButton);

    // Replay controls
    QHBoxLayout* replayControlsLayout = new QHBoxLayout();
    previousMoveButton = new QPushButton("←");
    nextMoveButton = new QPushButton("→");
    replaySlider = new QSlider(Qt::Horizontal);
    replaySlider->setEnabled(false);

    replayControlsLayout->addWidget(previousMoveButton);
    replayControlsLayout->addWidget(replaySlider);
    replayControlsLayout->addWidget(nextMoveButton);

    // Replay status
    replayStatusLabel = new QLabel("Select a game from history to replay");
    replayStatusLabel->setAlignment(Qt::AlignCenter);

    // Replay board
    QWidget* replayBoardWidget = new QWidget();
    replayBoardLayout = new QGridLayout(replayBoardWidget);
    replayBoardLayout->setSpacing(5);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            boardReplayButtons[row][col] = new QPushButton("");
            boardReplayButtons[row][col]->setFixedSize(60, 60);
            QFont buttonFont = boardReplayButtons[row][col]->font();
            buttonFont.setPointSize(18);
            buttonFont.setBold(true);
            boardReplayButtons[row][col]->setFont(buttonFont);
            boardReplayButtons[row][col]->setEnabled(false);

            replayBoardLayout->addWidget(boardReplayButtons[row][col], row, col);
        }
    }

    layout->addWidget(titleLabel);
    layout->addSpacing(10);
    layout->addWidget(gamesList);
    layout->addLayout(buttonsLayout);
    layout->addSpacing(20);
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
                button->setText("X");
                button->setStyleSheet("color: blue;");
            } else if (cell == Player::O) {
                button->setText("O");
                button->setStyleSheet("color: red;");
            } else {
                button->setText("");
                button->setStyleSheet("");
            }
        }
    }

    // Update game status label
    if (!gameLogic->isGameOver()) {
        Player currentPlayer = gameLogic->getCurrentPlayer();
        QString playerText = (currentPlayer == Player::X) ? "X" : "O";
        gameStatusLabel->setText("Player " + playerText + "'s Turn");
    }
}

void MainWindow::updateBoardButtons()
{
    // Update the board with the current game state
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Player cell = gameLogic->getCell(row, col);
            QPushButton* button = boardButtons[row][col];

            if (cell == Player::X) {
                button->setText("X");
                button->setStyleSheet("color: blue;");
            } else if (cell == Player::O) {
                button->setText("O");
                button->setStyleSheet("color: red;");
            } else {
                button->setText("");
                button->setStyleSheet("");
            }
        }
    }
}

void MainWindow::updateReplayBoardButtons()
{
    // Update the replay board with the current game state
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Player cell = gameLogic->getCell(row, col);
            QPushButton* button = boardReplayButtons[row][col];

            if (cell == Player::X) {
                button->setText("X");
                button->setStyleSheet("color: blue;");
            } else if (cell == Player::O) {
                button->setText("O");
                button->setStyleSheet("color: red;");
            } else {
                button->setText("");
                button->setStyleSheet("");
            }
        }
    }
}

void MainWindow::handleGameEnd(Player winner)
{
    QString resultText;

    if (winner == Player::None) {
        resultText = "Game Over - It's a Tie!";
    } else {
        QString winnerText = (winner == Player::X) ? "X" : "O";
        resultText = "Game Over - Player " + winnerText + " Wins!";
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
        welcomeLabel->setText("Welcome, " + userAuth->getCurrentUser() + "!");
        stackedWidget->setCurrentWidget(menuPage);
    } else {
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
        loginStatusLabel->setText("Please enter both username and password.");
        return;
    }

    if (userAuth->signIn(username, password)) {
        showMenuPage();
    } else {
        loginStatusLabel->setText("Invalid username or password.");
    }
}

void MainWindow::handleSignup() {
    QString username = signupUsername->text();
    QString password = signupPassword->text();
    QString confirmPassword = signupConfirmPassword->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        signupStatusLabel->setText("Please fill in all fields.");
        return;
    }

    if (password != confirmPassword) {
        signupStatusLabel->setText("Passwords do not match.");
        return;
    }

    // Check username format
    for (const QChar& ch : username) {
        if (!ch.isLetterOrNumber() && ch != '_') {
            signupStatusLabel->setText("Username can only contain letters, numbers, and underscores.");
            return;
        }
    }

    // Check password length
    if (password.length() < 8) {
        signupStatusLabel->setText("Password must be at least 8 characters long.");
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
        signupStatusLabel->setText("Password must contain at least one uppercase letter.");
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
        signupStatusLabel->setText("Password must contain at least one special character.");
        return;
    }

    if (userAuth->signUp(username, password)) {
        QMessageBox::information(this, "Success", "Account created successfully. Please log in.");
        showLoginPage();
    } else {
        signupStatusLabel->setText("Username already exists.");
    }
}

void MainWindow::handleLogout()
{
    userAuth->signOut();
    showLoginPage();
}

void MainWindow::startTwoPlayerGame()
{
    gameLogic->newGame(false); // Start game with 2 players
    showGamePage();
}

void MainWindow::startAIGame() {
    AIDifficulty difficulty = static_cast<AIDifficulty>(difficultyComboBox->currentData().toInt());
    gameLogic->setDifficulty(difficulty);
    gameLogic->newGame(true); // Start game with AI
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
        QString vsAI = gameData["vsAI"].toBool() ? "vs AI" : "vs Player";

        QListWidgetItem* item = new QListWidgetItem(QString("%1 - %2 (%3)").arg(date).arg(result).arg(vsAI));
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
        QString vsAI = gameData["vsAI"].toBool() ? "vs AI" : "vs Player";
        replayStatusLabel->setText(QString("Replaying game from %1 (%2)").arg(gameData["date"].toString()).arg(vsAI));

        // Update board
        updateReplayBoardButtons();
    }
}

void MainWindow::updateReplay(int value)
{
    gameLogic->replayMove(value);
    updateReplayBoardButtons();

    // Update status
    QVector<Move> moves = gameLogic->getMoves();
    if (value == 0) {
        replayStatusLabel->setText("Game start");
    } else if (value == moves.size()) {
        QString endText = "Game end - ";
        if (gameLogic->getWinner() == Player::None) {
            endText += "Tie";
        } else if (gameLogic->getWinner() == Player::X) {
            endText += "X wins";
        } else {
            endText += "O wins";
        }
        replayStatusLabel->setText(endText);
    } else {
        Player lastMovePlayer = (value % 2 == 1) ? Player::X : Player::O;
        QString playerText = (lastMovePlayer == Player::X) ? "X" : "O";
        replayStatusLabel->setText("Move " + QString::number(value) + " - Player " + playerText);
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

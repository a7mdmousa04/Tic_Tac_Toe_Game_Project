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
    

    // Add pages to stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(signupPage);
   

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
#include <QtTest>
#include "gamelogic.h"
#include "mainwindow.h"
#include "userauth.h"

// Test class for integration tests
class IntegrationTest : public QObject {
    Q_OBJECT
private slots:
    void testLoginAndStartAIGame();
    void testGameLogicVsAI();
    void testGameEndAndHistory();
};

void IntegrationTest::testLoginAndStartAIGame() {
    // Setup authentication
    UserAuth auth;
    QString username = "testuser";
    QString password = "Password1!";

    // Ensure user exists
    if (!auth.signIn(username, password)) {
        QVERIFY(auth.signUp(username, password));
        QVERIFY(auth.signIn(username, password));
    }
    QVERIFY(auth.isLoggedIn());
    QCOMPARE(auth.getCurrentUser(), username);

    // Setup GUI and start AI game
    MainWindow window(&auth);
    window.showMenuPage();
    window.showGameModePage();
    window.gameLogic->setDifficulty(AIDifficulty::Easy);
    window.startAIGame();

    // The game should be vs AI and not over
    QVERIFY(window.gameLogic->isVsAI());
    QVERIFY(!window.gameLogic->isGameOver());
}

void IntegrationTest::testGameLogicVsAI() {
    GameLogic logic;
    logic.setDifficulty(AIDifficulty::Medium);
    logic.newGame(true); // vs AI

    // Human moves
    QVERIFY(logic.makeMove(0, 0));
    // After human move, AI should move automatically
    // There should be at least 2 moves (X and O)
    QVERIFY(logic.getMoves().size() >= 2);

    // Check that board reflects moves
    Player first = logic.getCell(0, 0);
    QVERIFY(first == Player::X);
    bool aiMoved = false;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (!(r == 0 && c == 0) && logic.getCell(r, c) == Player::O)
                aiMoved = true;
    QVERIFY(aiMoved);
}

void IntegrationTest::testGameEndAndHistory() {
    UserAuth auth;
    QString username = "testuser2";
    QString password = "Password2!";

    // Ensure user exists and logged in
    if (!auth.signIn(username, password)) {
        QVERIFY(auth.signUp(username, password));
        QVERIFY(auth.signIn(username, password));
    }

    GameLogic logic;
    logic.setDifficulty(AIDifficulty::Easy);
    logic.newGame(true);

    // Play a full game (simulate moves until game over)
    while (!logic.isGameOver()) {
        // Find first empty cell and make move
        bool moved = false;
        for (int r = 0; r < 3 && !moved; ++r) {
            for (int c = 0; c < 3 && !moved; ++c) {
                if (logic.getCell(r, c) == Player::None) {
                    moved = logic.makeMove(r, c);
                }
            }
        }
        QVERIFY(moved);
    }
    QVERIFY(logic.isGameOver());

    // Save to history
    QJsonObject gameData = logic.getGameAsJson();
    QVERIFY(auth.saveGameToHistory(gameData));

    // Retrieve and check history
    QJsonArray history = auth.getGameHistory();
    QVERIFY(history.size() > 0);
    QJsonObject lastGame = history.last().toObject();
    QVERIFY(lastGame.contains("result"));
    QVERIFY(lastGame.contains("moves"));
    QVERIFY(lastGame["moves"].toArray().size() > 0);
}

QTEST_MAIN(IntegrationTest)
#include "integration_tests.moc"

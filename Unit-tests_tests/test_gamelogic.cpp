#include "test_gamelogic.h"
#include <QSignalSpy>

void TestGameLogic::initTestCase()
{
    // Called before the first test function is executed
}

void TestGameLogic::cleanupTestCase()
{
    // Called after the last test function was executed
}

void TestGameLogic::init()
{
    // Called before each test function is executed
    gameLogic = new GameLogic(this);
}

void TestGameLogic::cleanup()
{
    // Called after every test function
    delete gameLogic;
    gameLogic = nullptr;
}

void TestGameLogic::testNewGame()
{
    // Test new game initialization
    gameLogic->newGame(false);

    QCOMPARE(gameLogic->getCurrentPlayer(), Player::X);
    QCOMPARE(gameLogic->getWinner(), Player::None);
    QCOMPARE(gameLogic->isGameOver(), false);
    QCOMPARE(gameLogic->isVsAI(), false);

    // Check all cells are empty
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QCOMPARE(gameLogic->getCell(i, j), Player::None);
        }
    }
}

void TestGameLogic::testMakeMove()
{
    gameLogic->newGame(false);

    // Test valid move
    bool result = gameLogic->makeMove(0, 0);
    QCOMPARE(result, true);
    QCOMPARE(gameLogic->getCell(0, 0), Player::X);
    QCOMPARE(gameLogic->getCurrentPlayer(), Player::O);

    // Test another valid move
    result = gameLogic->makeMove(1, 1);
    QCOMPARE(result, true);
    QCOMPARE(gameLogic->getCell(1, 1), Player::O);
    QCOMPARE(gameLogic->getCurrentPlayer(), Player::X);
}

void TestGameLogic::testMakeMoveInvalid()
{
    gameLogic->newGame(false);

    // Test invalid coordinates
    QCOMPARE(gameLogic->makeMove(-1, 0), false);
    QCOMPARE(gameLogic->makeMove(0, -1), false);
    QCOMPARE(gameLogic->makeMove(3, 0), false);
    QCOMPARE(gameLogic->makeMove(0, 3), false);

    // Test occupied cell
    gameLogic->makeMove(0, 0);
    QCOMPARE(gameLogic->makeMove(0, 0), false);
}

void TestGameLogic::testWinConditions()
{
    gameLogic->newGame(false);

    // Test horizontal win
    gameLogic->makeMove(0, 0); // X
    gameLogic->makeMove(1, 0); // O
    gameLogic->makeMove(0, 1); // X
    gameLogic->makeMove(1, 1); // O

    QSignalSpy spy(gameLogic, &GameLogic::gameEnded);
    gameLogic->makeMove(0, 2); // X wins

    QCOMPARE(gameLogic->isGameOver(), true);
    QCOMPARE(gameLogic->getWinner(), Player::X);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Player>(), Player::X);
}

void TestGameLogic::testTieGame()
{
    gameLogic->newGame(false);

    // Create a tie game
    gameLogic->makeMove(0, 0); // X
    gameLogic->makeMove(0, 1); // O
    gameLogic->makeMove(0, 2); // X
    gameLogic->makeMove(1, 0); // O
    gameLogic->makeMove(1, 1); // X
    gameLogic->makeMove(2, 0); // O
    gameLogic->makeMove(1, 2); // X
    gameLogic->makeMove(2, 2); // O

    QSignalSpy spy(gameLogic, &GameLogic::gameEnded);
    gameLogic->makeMove(2, 1); // X - Board full, tie

    QCOMPARE(gameLogic->isGameOver(), true);
    QCOMPARE(gameLogic->getWinner(), Player::None);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Player>(), Player::None);
}

void TestGameLogic::testCurrentPlayer()
{
    gameLogic->newGame(false);

    QCOMPARE(gameLogic->getCurrentPlayer(), Player::X);
    gameLogic->makeMove(0, 0);
    QCOMPARE(gameLogic->getCurrentPlayer(), Player::O);
    gameLogic->makeMove(1, 1);
    QCOMPARE(gameLogic->getCurrentPlayer(), Player::X);
}

void TestGameLogic::testGameOver()
{
    gameLogic->newGame(false);

    QCOMPARE(gameLogic->isGameOver(), false);

    // Make moves until game ends
    gameLogic->makeMove(0, 0); // X
    gameLogic->makeMove(1, 0); // O
    gameLogic->makeMove(0, 1); // X
    gameLogic->makeMove(1, 1); // O
    gameLogic->makeMove(0, 2); // X wins

    QCOMPARE(gameLogic->isGameOver(), true);
}

void TestGameLogic::testAIMove()
{
    gameLogic->newGame(true); // vs AI
    gameLogic->setDifficulty(AIDifficulty::Unbeatable);

    QCOMPARE(gameLogic->isVsAI(), true);

    // Make a move as human player
    gameLogic->makeMove(0, 0); // X

    // AI should have made a move automatically
    // Check that at least one more cell is occupied
    int occupiedCells = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) != Player::None) {
                occupiedCells++;
            }
        }
    }
    QCOMPARE(occupiedCells, 2); // Human move + AI move
}

void TestGameLogic::testReplay()
{
    gameLogic->newGame(false);

    // Make some moves
    gameLogic->makeMove(0, 0); // X
    gameLogic->makeMove(1, 1); // O
    gameLogic->makeMove(0, 1); // X

    QVector<Move> moves = gameLogic->getMoves();
    QCOMPARE(moves.size(), 3);

    // Test replay to different positions
    gameLogic->replayMove(1);
    QCOMPARE(gameLogic->getCell(0, 0), Player::X);
    QCOMPARE(gameLogic->getCell(1, 1), Player::None);
    QCOMPARE(gameLogic->getCell(0, 1), Player::None);

    // Reset replay
    gameLogic->resetReplay();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QCOMPARE(gameLogic->getCell(i, j), Player::None);
        }
    }
}

void TestGameLogic::testJsonSerialization()
{
    gameLogic->newGame(true);
    gameLogic->setDifficulty(AIDifficulty::Hard);

    // Make some moves
    gameLogic->makeMove(0, 0);
    gameLogic->makeMove(1, 1);

    // Get JSON representation
    QJsonObject json = gameLogic->getGameAsJson();

    QCOMPARE(json["vsAI"].toBool(), true);
    QCOMPARE(json["difficulty"].toString(), QString("Hard"));
    QVERIFY(json.contains("moves"));
    QVERIFY(json.contains("result"));

    // Test loading from JSON
    GameLogic newGame;
    newGame.loadFromJson(json);

    QCOMPARE(newGame.isVsAI(), true);
    QCOMPARE(newGame.getDifficulty(), AIDifficulty::Hard);
}

void TestGameLogic::testDifficulty()
{
    gameLogic->setDifficulty(AIDifficulty::Easy);
    QCOMPARE(gameLogic->getDifficulty(), AIDifficulty::Easy);

    gameLogic->setDifficulty(AIDifficulty::Hard);
    QCOMPARE(gameLogic->getDifficulty(), AIDifficulty::Hard);

    gameLogic->setDifficulty(AIDifficulty::Unbeatable);
    QCOMPARE(gameLogic->getDifficulty(), AIDifficulty::Unbeatable);
}

// Register the test class
QTEST_MAIN(TestGameLogic)


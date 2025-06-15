#ifndef TEST_GAMELOGIC_H
#define TEST_GAMELOGIC_H

#include <QTest>
#include <QObject>
#include "gamelogic.h"

class TestGameLogic : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test functions
    void testNewGame();
    void testMakeMove();
    void testMakeMoveInvalid();
    void testWinConditions();
    void testTieGame();
    void testCurrentPlayer();
    void testGameOver();
    void testAIMove();
    void testReplay();
    void testJsonSerialization();
    void testDifficulty();

private:
    GameLogic *gameLogic;
};

#endif // TEST_GAMELOGIC_H

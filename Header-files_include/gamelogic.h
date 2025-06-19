// gamelogic.h - Game Logic Implementation
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

enum class Player {
    None,
    X,
    O
};
enum class AIDifficulty {
    Easy,     // Makes optimal move 30% of the time
    Medium,   // Makes optimal move 50% of the time
    Hard,     // Makes optimal move 80% of the time
    Unbeatable // Makes optimal move 100% of the time
};

struct Move {
    int row;
    int col;
    Player player;
};

class GameLogic : public QObject {
    Q_OBJECT

public:
    explicit GameLogic(QObject *parent = nullptr);
    void newGame(bool vsAI);
    bool makeMove(int row, int col);
    Player getCell(int row, int col) const;
    Player getCurrentPlayer() const;
    Player getWinner() const;
    bool isGameOver() const;
    bool isVsAI() const;
    void aiMove();
    QJsonObject getGameAsJson() const;
    void replayMove(int index);
    void resetReplay();
    QVector<Move> getMoves() const;
    void loadFromJson(const QJsonObject& gameData);
    void setDifficulty(AIDifficulty difficulty);
    AIDifficulty getDifficulty() const;
private:
    QVector<QVector<Player>> board;
    Player currentPlayer;
    Player winner;
    bool gameOver;
    bool vsAI;
    QVector<Move> moves;
    int replayIndex;
    QDateTime startTime;
    AIDifficulty aiDifficulty;
    bool shouldUseOptimalMove();

    void switchPlayer();
    bool checkWin(int row, int col);
    bool checkGameOver();
    void makeAIMove();
    int minimax(QVector<QVector<Player>>& board, int depth, bool isMaximizing, int alpha, int beta);
    QVector<QPair<int, int>> getAvailableMoves(const QVector<QVector<Player>>& board) const;
    bool isWin(const QVector<QVector<Player>>& board, Player player) const;

signals:
    void boardChanged();
    void gameEnded(Player winner);
};

#endif // GAMELOGIC_H

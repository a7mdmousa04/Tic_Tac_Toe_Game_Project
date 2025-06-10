

// gamelogic.cpp - Game Logic Implementation
#include "gamelogic.h"
#include <QRandomGenerator>
#include <QJsonArray>
#include <QJsonObject>

GameLogic::GameLogic(QObject* parent) : QObject(parent),
gameOver(false), vsAI(false), replayIndex(0), aiDifficulty(AIDifficulty::Medium) {
    // Initialize the board
    board.resize(3);
    for (int i = 0; i < 3; ++i) {
        board[i].resize(3, Player::None);
    }
    // Set starting player to X
    currentPlayer = Player::X;
}
void GameLogic::newGame(bool vsAI) {
    // Clear the board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = Player::None;
        }
    }

    // Reset game state
    currentPlayer = Player::X;
    winner = Player::None;
    gameOver = false;
    this->vsAI = vsAI;
    moves.clear();
    replayIndex = 0;
    startTime = QDateTime::currentDateTime();

    emit boardChanged();
}

bool GameLogic::makeMove(int row, int col) {
    // Check if move is valid
    if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != Player::None || gameOver) {
        return false;
    }

    // Make the move
    board[row][col] = currentPlayer;

    // Record the move
    Move move;
    move.row = row;
    move.col = col;
    move.player = currentPlayer;
    moves.append(move);

    // Check if game is over
    if (checkWin(row, col)) {
        winner = currentPlayer;
        gameOver = true;
        emit gameEnded(winner);
    }
    else if (checkGameOver()) {
        gameOver = true;
        emit gameEnded(Player::None); // Tie
    }
    else {
        // Switch player
        switchPlayer();

        // If playing against AI and it's AI's turn
        if (vsAI && currentPlayer == Player::O && !gameOver) {
            aiMove();
        }
    }

    emit boardChanged();
    return true;
}
void GameLogic::setDifficulty(AIDifficulty difficulty) {
    aiDifficulty = difficulty;
}

AIDifficulty GameLogic::getDifficulty() const {
    return aiDifficulty;
}

bool GameLogic::shouldUseOptimalMove() {
    int probability;
    switch (aiDifficulty) {
    case AIDifficulty::Easy:
        probability = 30;
        break;
    case AIDifficulty::Medium:
        probability = 50;
        break;
    case AIDifficulty::Hard:
        probability = 80;
        break;
    case AIDifficulty::Unbeatable:
        probability = 100;
        break;
    default:
        probability = 50;
    }

    // Generate a random number between 0 and 99
    int randomNum = QRandomGenerator::global()->bounded(100);
    return randomNum < probability;
}
void GameLogic::aiMove() {
    // AI uses minimax to make a move
    makeAIMove();
}

void GameLogic::makeAIMove() {
    if (shouldUseOptimalMove()) {
        // Use minimax for optimal move
        int bestScore = -1000;
        int bestRow = -1;
        int bestCol = -1;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Player::None) {
                    // Try this move
                    board[i][j] = Player::O;
                    // Get score from minimax
                    int score = minimax(board, 0, false, -1000, 1000);
                    // Undo the move
                    board[i][j] = Player::None;
                    // Update best move if needed
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        // Make the best move
        if (bestRow != -1 && bestCol != -1) {
            makeMove(bestRow, bestCol);
        }
    }
    else {
        // Make a random move
        QVector<QPair<int, int>> availableMoves = getAvailableMoves(board);
        if (!availableMoves.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(availableMoves.size());
            QPair<int, int> randomMove = availableMoves[randomIndex];
            makeMove(randomMove.first, randomMove.second);
        }
    }
}

int GameLogic::minimax(QVector<QVector<Player>>& board, int depth, bool isMaximizing, int alpha, int beta) {
    // Check for terminal states
    if (isWin(board, Player::O)) {
        return 10 - depth; // AI wins
    }
    if (isWin(board, Player::X)) {
        return depth - 10; // Human wins
    }

    // Check for a tie
    bool isFull = true;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == Player::None) {
                isFull = false;
                break;
            }
        }
        if (!isFull) break;
    }

    if (isFull) {
        return 0; // Tie
    }

    // Maximizing player (AI)
    if (isMaximizing) {
        int bestScore = -1000;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Player::None) {
                    // Try this move
                    board[i][j] = Player::O;

                    // Recurse
                    int score = minimax(board, depth + 1, false, alpha, beta);

                    // Undo the move
                    board[i][j] = Player::None;

                    // Update best score
                    bestScore = qMax(score, bestScore);

                    // Alpha-beta pruning
                    alpha = qMax(alpha, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        return bestScore;
    }
    // Minimizing player (Human)
    else {
        int bestScore = 1000;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Player::None) {
                    // Try this move
                    board[i][j] = Player::X;

                    // Recurse
                    int score = minimax(board, depth + 1, true, alpha, beta);

                    // Undo the move
                    board[i][j] = Player::None;

                    // Update best score
                    bestScore = qMin(score, bestScore);

                    // Alpha-beta pruning
                    beta = qMin(beta, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        return bestScore;
    }
}
bool GameLogic::isWin(const QVector<QVector<Player>>& board, Player player) const {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }

    return false;
}

QVector<QPair<int, int>> GameLogic::getAvailableMoves(const QVector<QVector<Player>>& board) const {
    QVector<QPair<int, int>> moves;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == Player::None) {
                moves.append(qMakePair(i, j));
            }
        }
    }

    return moves;
}

int GameLogic::evaluateBoard(const QVector<QVector<Player>>& board) {
    // This is a simple evaluation function
    // Positive score favors AI (O), negative score favors Human (X)

    // Check for wins
    if (isWin(board, Player::O)) {
        return 10;
    }
    if (isWin(board, Player::X)) {
        return -10;
    }

    return 0; // Neutral position
}

Player GameLogic::getCell(int row, int col) const {
    if (row >= 0 && row < 3 && col >= 0 && col < 3) {
        return board[row][col];
    }
    return Player::None;
}

Player GameLogic::getCurrentPlayer() const {
    return currentPlayer;
}

Player GameLogic::getWinner() const {
    return winner;
}

bool GameLogic::isGameOver() const {
    return gameOver;
}

bool GameLogic::isVsAI() const {
    return vsAI;
}

void GameLogic::switchPlayer() {
    currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
}

bool GameLogic::checkWin(int row, int col) {
    Player p = board[row][col];

    // Check row
    if (board[row][0] == p && board[row][1] == p && board[row][2] == p) {
        return true;
    }

    // Check column
    if (board[0][col] == p && board[1][col] == p && board[2][col] == p) {
        return true;
    }

    // Check diagonals
    if (row == col) {
        if (board[0][0] == p && board[1][1] == p && board[2][2] == p) {
            return true;
        }
    }

    if (row + col == 2) {
        if (board[0][2] == p && board[1][1] == p && board[2][0] == p) {
            return true;
        }
    }

    return false;
}

bool GameLogic::checkGameOver() {
    // Check if board is full
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == Player::None) {
                return false;
            }
        }
    }

    return true;
}

QJsonObject GameLogic::getGameAsJson() const {
    QJsonObject gameData;

    // Game metadata
    gameData["date"] = startTime.toString(Qt::ISODate);
    gameData["vsAI"] = vsAI;

    // Add difficulty level
    if (vsAI) {
        QString difficultyStr;
        switch (aiDifficulty) {
        case AIDifficulty::Easy: difficultyStr = "Easy"; break;
        case AIDifficulty::Medium: difficultyStr = "Medium"; break;
        case AIDifficulty::Hard: difficultyStr = "Hard"; break;
        case AIDifficulty::Unbeatable: difficultyStr = "Unbeatable"; break;
        }
        gameData["difficulty"] = difficultyStr;
    }

    // Game moves
    QJsonArray movesArray;
    for (const auto& move : moves) {
        QJsonObject moveObj;
        moveObj["row"] = move.row;
        moveObj["col"] = move.col;
        moveObj["player"] = (move.player == Player::X) ? "X" : "O";
        movesArray.append(moveObj);
    }
    gameData["moves"] = movesArray;

    // Game result
    QString result;
    if (winner == Player::None && gameOver) {
        result = "Tie";
    } else if (winner == Player::X) {
        result = "X wins";
    } else if (winner == Player::O) {
        result = "O wins";
    } else {
        result = "Incomplete";
    }
    gameData["result"] = result;

    return gameData;
}


void GameLogic::replayMove(int index) {
    if (index < 0 || index > moves.size()) {
        return;
    }

    // Reset the board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = Player::None;
        }
    }

    // Play up to the specified move
    for (int i = 0; i < index; ++i) {
        const Move& move = moves[i];
        board[move.row][move.col] = move.player;
    }

    replayIndex = index;

    // Set current player based on replay state
    if (index % 2 == 0) {
        currentPlayer = Player::X;
    } else {
        currentPlayer = Player::O;
    }

    // Set game over status
    if (index == moves.size()) {
        gameOver = isGameOver();
    } else {
        gameOver = false;
        winner = Player::None;
    }

    emit boardChanged();
}

void GameLogic::resetReplay() {
    replayIndex = 0;
    replayMove(0);
}

QVector<Move> GameLogic::getMoves() const {
    return moves;
}

void GameLogic::loadFromJson(const QJsonObject& gameData) {
    // Clear the board and reset game state
    newGame(gameData["vsAI"].toBool());

    // Set difficulty if available
    if (gameData.contains("difficulty")) {
        QString difficultyStr = gameData["difficulty"].toString();
        if (difficultyStr == "Easy") {
            aiDifficulty = AIDifficulty::Easy;
        } else if (difficultyStr == "Medium") {
            aiDifficulty = AIDifficulty::Medium;
        } else if (difficultyStr == "Hard") {
            aiDifficulty = AIDifficulty::Hard;
        } else if (difficultyStr == "Unbeatable") {
            aiDifficulty = AIDifficulty::Unbeatable;
        }
    }

    // Load moves
    QJsonArray movesArray = gameData["moves"].toArray();
    moves.clear();
    for (const QJsonValue& value : movesArray) {
        QJsonObject moveObj = value.toObject();
        Move move;
        move.row = moveObj["row"].toInt();
        move.col = moveObj["col"].toInt();
        move.player = moveObj["player"].toString() == "X" ? Player::X : Player::O;
        moves.append(move);
    }

    // Execute all moves to recreate the final state
    replayMove(moves.size());
}

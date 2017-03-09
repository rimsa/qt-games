#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <Ball.h>

class Game : public QObject {
    Q_OBJECT
    Q_ENUMS(Direction)

public:
    enum Direction {
        HORIZONTAL, VERTICAL, DIAGONAL, CROSS_DIAGONAL
    };

    explicit Game(int count, QObject *parent = 0);
    virtual ~Game();

    inline int count() const { return m_count; }
    inline int rows() const { return m_rows; }
    inline int cols() const { return m_cols; }

    inline Ball::Color turn() const { return m_turn; }

    static QString directionToString(Game::Direction direction);

signals:
    void victory(Ball::Color winner, Game::Direction direction);
    void columnFull(int col);

public slots:
    void addBall(Ball* ball);
    void play(int col);
    void reset();

private:
    int m_count;
    int m_rows;
    int m_cols;
    Ball::Color m_turn;
    Ball*** m_balls;

private slots:
    bool checkVictory(int row, int col);
    void nextTurn();

};

#endif // GAME_H

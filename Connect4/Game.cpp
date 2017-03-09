#include <QDebug>
#include <QWidget>

#include <Game.h>
#include <Ball.h>

Game::Game(int count, QObject *parent)
    : QObject(parent),
      m_count(count), m_rows(count+2), m_cols(count+3),
      m_turn(Ball::RED),
      m_balls(new Ball** [m_rows]) {

    for (int r = 0; r < m_rows; r++) {
        m_balls[r] = new Ball* [m_cols];
        for (int c = 0; c < m_cols; c++) {
            m_balls[r][c] = 0;
        }
    }
}

Game::~Game() {
  for (int r = 0; r < m_rows; r++) {
      delete m_balls[r];
  }

  delete[] m_balls;
}

QString Game::directionToString(Game::Direction direction) {
    switch (direction) {
        case Game::HORIZONTAL:
            return trUtf8("Horizontal");
        case Game::VERTICAL:
            return trUtf8("Vertical");
        case Game::DIAGONAL:
            return trUtf8("Diagonal Principal");
        case Game::CROSS_DIAGONAL:
            return trUtf8("Diagonal Invertida");
        default:
            // Unrecheable!
            abort();
    }
}

void Game::addBall(Ball *ball) {
    static int count = 0;

    Q_ASSERT(count < (m_cols * m_rows));

    int r = (count / m_cols);
    int c = (count % m_cols);

    m_balls[r][c] = ball;

    count++;
}

void Game::play(int col) {
    Q_ASSERT(col >= 0 && col < m_cols);

    int row;
    Ball* ball = 0;
    for (row = 0; row < m_rows; row++) {
        if (m_balls[row][col]->isBlank()) {
            ball = m_balls[row][col];
            break;
        }
    }

    if (ball) {
        ball->setColor(m_turn);

        if (!this->checkVictory(row, col)) {
            this->nextTurn();

            if (row == (m_rows - 1))
                emit columnFull(col);
        }
    }
}

void Game::reset() {
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            m_balls[r][c]->reset();
        }
    }
}

bool Game::checkVictory(int row, int col) {
    int count;
    int r, c;

    // Check victory on the horizontal
    {
        count = 1;

        c = col;
        while (++c < m_cols && m_balls[row][c]->color() == m_turn)
            count++;

        c = col;
        while (--c >= 0 && m_balls[row][c]->color() == m_turn)
            count++;

        if (count >= m_count) {
            emit victory(m_turn, Game::HORIZONTAL);
            return true;
        }
    }

    // Check victory on the vertical
    {
        count = 1;

        r = row;
        while (++r < m_rows && m_balls[r][col]->color() == m_turn)
            count++;

        r = row;
        while (--r >= 0 && m_balls[r][col]->color() == m_turn)
            count++;

        if (count >= m_count) {
            emit victory(m_turn, Game::VERTICAL);
            return true;
        }
    }

    // Check victory on the diagonal
    {
        count = 1;

        r = row;
        c = col;
        while (++r < m_rows && ++c < m_cols && m_balls[r][c]->color() == m_turn)
            count++;

        r = row;
        c = col;
        while (--r >= 0 && --c >= 0 && m_balls[r][c]->color() == m_turn)
            count++;

        if (count >= m_count) {
            emit victory(m_turn, Game::DIAGONAL);
            return true;
        }
    }

    // Check victory on the cross diagonal
    {
        count = 1;

        r = row;
        c = col;
        while (--r >= 0 && ++c < m_cols && m_balls[r][c]->color() == m_turn)
            count++;

        r = row;
        c = col;
        while (++r < m_rows && --c >= 0 && m_balls[r][c]->color() == m_turn)
            count++;

        if (count >= m_count) {
            emit victory(m_turn, Game::CROSS_DIAGONAL);
            return true;
        }
    }

    return false;
}

void Game::nextTurn() {
    if (m_turn == Ball::RED) {
        m_turn = Ball::BLUE;
    } else if (m_turn == Ball::BLUE) {
        m_turn = Ball::RED;
    } else {
        // Unrecheable!
        abort();
    }
}

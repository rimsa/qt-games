#ifndef CONNECT4_H
#define CONNECT4_H

#include <QList>
#include <QMainWindow>

#include <Ball.h>
#include <Game.h>

namespace Ui {
    class Connect4;
}

class QPushButton;

class Connect4 : public QMainWindow {
    Q_OBJECT

public:
    explicit Connect4(QWidget *parent = 0);
    virtual ~Connect4();

public slots:
    void reset();

private:
    Ui::Connect4 *ui;
    Game* m_game;
    QList<QPushButton*> m_buttons;

private slots:
    void initGame();
    void disableColumn(int col);
    void showWinner(Ball::Color color, Game::Direction direction);
    void showTie();

};

#endif // CONNECT4_H

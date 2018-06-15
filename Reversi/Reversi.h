#ifndef REVERSI_H
#define REVERSI_H

#include <QMainWindow>

class Cell;
class Player;

namespace Ui {
    class Reversi;
}

class Reversi : public QMainWindow {
    Q_OBJECT

public:
    enum Coordinates {
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest
    };
    Q_ENUMS(Coordinates)

    explicit Reversi(QWidget *parent = 0);
    virtual ~Reversi();

public slots:
    void play();

private:
    Ui::Reversi *ui;
    Cell* m_board[8][8];
    Player* m_player;

    bool canPlay(Cell* cell);
    bool canPlayOnCoords(Cell* cell, Reversi::Coordinates coords);
    bool isGameOver();

private slots:
    void reset();

    void updateStatus();
    void showAbout();
    void showGameOver();

    void flipOnCoords(Cell* cell, Reversi::Coordinates coords);

};

#endif // REVERSI_H

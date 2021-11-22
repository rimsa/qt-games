#ifndef SLIMETRAIL_H
#define SLIMETRAIL_H

#include <QMainWindow>

#include "Hole.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SlimeTrail;
}
QT_END_NAMESPACE

class SlimeTrail : public QMainWindow {
    Q_OBJECT

public:
    enum Player {
        RedPlayer,
        BluePlayer
    };
    Q_ENUM(Player)

    SlimeTrail(QWidget *parent = nullptr);
    virtual ~SlimeTrail();

signals:
    void turnEnded();
    void gameOver();

private:
    Ui::SlimeTrail *ui;
    Player m_player;
    Hole* m_current;
    Hole* m_board[8][8];

    Hole* neighboor(Hole* hole, Hole::Direction dir);
    int markNeighboors(Hole* hole, bool mark = true);

private slots:
    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void showGameOver();

    void updateStatusBar();

};

#endif // SLIMETRAIL_H

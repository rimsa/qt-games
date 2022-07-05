#ifndef ALQUERQUE_H
#define ALQUERQUE_H

#include <QMainWindow>
#include <QMap>
#include <QSet>
#include "Hole.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Alquerque;
}
QT_END_NAMESPACE

class Alquerque : public QMainWindow {
    Q_OBJECT

public:
    enum Player {
        RedPlayer,
        BluePlayer
    };
    Q_ENUM(Player)

    enum Mode {
        MovingMode,
        EatingMode
    };
    Q_ENUM(Mode)

    Alquerque(QWidget *parent = nullptr);
    virtual ~Alquerque();

signals:
    void turnEnded();
    void gameOver(Alquerque::Player player);

private:
    Ui::Alquerque *ui;
    Player m_player;
    Mode m_mode;
    Hole* m_selected;
    bool m_sequence;
    QMap<Hole*, QSet<Hole::Direction>> m_movements;
    Hole* m_board[5][5];

    Hole* neighboor(Hole* hole, Hole::Direction dir);

    Hole* move(Hole* src, Hole::Direction dir, bool fake = false);
    QList<Hole::Direction> moveables(Hole* hole);

    Hole* eat(Hole* src, Hole::Direction dir, bool fake = false);
    QList<Hole::Direction> eatables(Hole* hole);

    Hole::Direction findDirection(Hole* hole);
    void select(Hole* hole);
    void deselect();
    bool nextSequence(Hole* hole);
    bool preplay();

private slots:
    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void showGameOver(Alquerque::Player player);
    void updateStatusBar();

};

#endif // ALQUERQUE_H

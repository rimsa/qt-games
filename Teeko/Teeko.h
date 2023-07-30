#ifndef TEEKO_H
#define TEEKO_H

#include <QMainWindow>
#include <QMap>
#include <QSet>
#include "Hole.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Teeko;
}
QT_END_NAMESPACE

class Teeko : public QMainWindow {
    Q_OBJECT

public:
    enum Phase {
        DropPhase,
        MovePhase
    };
    Q_ENUM(Phase)

    Teeko(QWidget *parent = nullptr);
    virtual ~Teeko();

    Teeko::Phase phase() const { return m_phase; }

signals:
    void phaseChanged(Teeko::Phase phase);
    void turnEnded();
    void gameOver();

private:
    Ui::Teeko *ui;
    Player* m_player;
    Phase m_phase;
    int m_dropCount;
    Hole* m_selected;
    Hole* m_board[5][5];

    void drop(Hole* hole);
    void move(Hole* hole);
    void clearPreviousPlay(void);

    Hole* neighboor(Hole* hole, Hole::Direction dir) const;
    bool checkWinner() const;
    bool checkSequence(Hole* hole, Hole::Direction dir) const;
    bool checkSquare(Hole* hole) const;

private slots:
    void setPhase(Teeko::Phase phase);

    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void showGameOver();
    void updateStatusBar();

};

#endif // TEEKO_H

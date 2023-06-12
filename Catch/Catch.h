#ifndef CATCH_H
#define CATCH_H

#include <QMainWindow>
#include <QList>
#include <QSet>

#include "Cell.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Catch;
}
QT_END_NAMESPACE

class Player;

class Catch : public QMainWindow {
    Q_OBJECT

public:
    Catch(QWidget *parent = nullptr);
    virtual ~Catch();

signals:
    void turnEnded();
    void gameOver();

private:
    Ui::Catch *ui;
    Player* m_player;
    Cell* m_board[8][8];

    Cell* neighboor(Cell* cell, Cell::Direction dir);
    QList<QSet<Cell*> > findClusters();

private slots:
    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void showGameOver();

    void updateSelectables(bool over);
    void updateCount(Player* player);
    void updateStatusBar();

    void checkGameOver();

};

#endif // CATCH_H

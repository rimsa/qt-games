#ifndef SQUADRO_H
#define SQUADRO_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Squadro; }
QT_END_NAMESPACE

class Cell;
class Player;

class Squadro : public QMainWindow {
    Q_OBJECT

public:
    Squadro(QWidget *parent = nullptr);
    virtual ~Squadro();

signals:
    void turnEnded();
    void gameOver();

private:
    Ui::Squadro *ui;
    Player* m_player;
    Cell* m_board[7][7];

private slots:
    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void showGameOver();
    void updateStatusBar();

};

#endif // SQUADRO_H

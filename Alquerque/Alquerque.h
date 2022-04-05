#ifndef ALQUERQUE_H
#define ALQUERQUE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Alquerque;
}
QT_END_NAMESPACE

class Hole;

class Alquerque : public QMainWindow {
    Q_OBJECT

public:
    enum Player {
        RedPlayer,
        BluePlayer
    };
    Q_ENUM(Player)

    Alquerque(QWidget *parent = nullptr);
    virtual ~Alquerque();

signals:
    void turnEnded();

private:
    Ui::Alquerque *ui;
    Player m_player;
    Hole* m_board[5][5];

private slots:
    void play(int id);
    void switchPlayer();
    void reset();

    void showAbout();
    void updateStatusBar();

};

#endif // ALQUERQUE_H

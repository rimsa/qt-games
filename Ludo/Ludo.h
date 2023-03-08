#ifndef LUDO_H
#define LUDO_H

#include <QMainWindow>

namespace Ui {
    class Ludo;
}

class Pawn;
class Player;

class Ludo : public QMainWindow {
    Q_OBJECT

public:
    enum Action {
        Roll,
        Move
    };
    Q_ENUM(Action)

    explicit Ludo(QWidget *parent = 0);
    virtual ~Ludo();

    Player* current() { return m_players[m_current]; }
    int count() const { return m_count; }
    Ludo::Action action() const { return m_action; }

public slots:
    void reset();

signals:
    void countChanged(int count);
    void actionChanged(Ludo::Action action);
    void turnEnded();
    void gameOver(Player* player);

private:
    Ui::Ludo *ui;
    int m_current;
    int m_count;
    bool m_return;
    int m_sixes;
    Ludo::Action m_action;
    Player* m_players[4];

private slots:
    void changeMode(QAction* action);
    void setAction(Ludo::Action action);

    void showAbout();
    void showGameOver(Player* player);

    void handleRoll(int n);
    void handleAction(Ludo::Action action);
    void switchPlayer();

    void movePawnFromHome(Pawn* pawn);
    void returnPawnToHome(Pawn* pawn);
    void movePawnOnTrack(Pawn* pawn);
    void movePawnToStatus(Pawn* pawn);

};

#endif // LUDO_H

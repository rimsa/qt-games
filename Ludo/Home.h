#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <Qset>
#include "Player.h"

namespace Ui {
    class Home;
}

class Tile;

class Home : public QWidget {
    Q_OBJECT
    Q_PROPERTY(Player::Type type READ type WRITE setType NOTIFY typeChanged)

public:
    explicit Home(QWidget *parent = 0);
    virtual ~Home();

    Player::Type type() const { return m_player->type(); }
    Player* player() const { return m_player; }
    const QSet<Pawn*> pawns() const { return m_pawns; }

    bool markPlayablePawns(int roll);

public slots:
    void setType(Player::Type type);
    void insertPawn(Pawn* pawn);
    void removePawn(Pawn* pawn);
    void reset();

signals:
    void typeChanged(Player::Type type);
    void pawnSelected(Pawn* pawn);

private:
    Ui::Home *ui;
    Player* m_player;
    Tile* m_tiles[4];
    QSet<Pawn*> m_pawns;

private slots:
    void handleClick();

};

#endif // HOME_H

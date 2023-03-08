#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QLabel>

#include "Player.h"

namespace Ui {
    class Status;
}

class Tile;

class Status : public QWidget {
    Q_OBJECT
    Q_PROPERTY(Player::Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool turn READ isTurn WRITE setTurn NOTIFY turnChanged)

public:
    explicit Status(QWidget *parent = nullptr);
    virtual ~Status();

    Player::Type type() const { return m_player->type(); }
    Player* player() const { return m_player; }
    bool isTurn() const { return m_turn; }

    int finished() const { return m_pawns.count(); }

public slots:
    void setType(Player::Type type);
    void setTurn(bool turn);
    void insertPawn(Pawn* pawn);
    void reset();

signals:
    void typeChanged(Player::Type type);
    void turnChanged(bool turn);
    void pawnInserted(Pawn* pawn);

private:
    Ui::Status *ui;
    Player* m_player;
    bool m_turn;
    QList<Pawn*> m_pawns;
    Tile* m_finish[4];

private slots:
    void updateStatus();

};

#endif // STATUS_H

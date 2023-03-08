#ifndef DICE_H
#define DICE_H

#include <QPushButton>
#include "Player.h"

class Dice : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(Player::Type type READ type WRITE setType NOTIFY typeChanged)

public:
    explicit Dice(QWidget *parent = 0);
    virtual ~Dice();

    Player::Type type() const { return m_player->type(); }
    Player* player() const { return m_player; }
    int last() const { return m_last; }

public slots:
    void setType(Player::Type type);
    void reset();

signals:
    void typeChanged(Player::Type type);
    void rolled(int n);

private:
    int m_last;
    Player* m_player;
    QPixmap m_faces[6];
    static QString sscode;

private slots:
    void roll();
    void updatePlayerColor();

};

#endif // DICE_H

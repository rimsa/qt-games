#ifndef PAWN_H
#define PAWN_H

#include <QObject>

class Player;
class Tile;

class Pawn : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool playable READ isPlayable WRITE setPlayable NOTIFY playableChanged)

public:
    explicit Pawn(Player* player);
    virtual ~Pawn();

    Player* player() const { return m_player; }
    bool isPlayable() const { return m_playable; }
    Tile* owner() const { return m_owner; }

    const QPixmap& pixmap() const;

public slots:
    void setPlayable(bool playable);
    void setOwner(Tile* owner);

signals:
    void playableChanged(bool playable);

    void pawnInsertedTo(Tile* tile);
    void pawnRemovedFrom(Tile* tile);

    void raised();

private:
    Player* m_player;
    bool m_playable;
    Tile* m_owner;

};

#endif // PAWN_H

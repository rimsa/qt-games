#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QMap>

class Tile;
class Pawn;
class Player;

class Track : public QObject {
    Q_OBJECT

public:
    explicit Track(Player* player, QObject *parent = nullptr);
    virtual ~Track();

    Player* player() const { return m_player; }
    const QList<Pawn*> pawns() const { return m_pawns.keys(); }

    const QList<Tile*>& tiles() const { return m_tiles; }
    void addTile(Tile* tile);

    bool markPlayablePawns(int roll);
    bool advance(Pawn* pawn, int steps);

public slots:
    void insertPawn(Pawn* pawn);
    void removePawn(Pawn* pawn);
    void reset();

signals:
    void pawnSelected(Pawn* pawn);
    void pawnReturned(Pawn* pawn);
    void finished(Pawn* pawn);

private:
    Player* m_player;
    QList<Tile*> m_tiles;
    QMap<Pawn*, int> m_pawns;

private slots:
    void handleClick();

};

#endif // TRACK_H

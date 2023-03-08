#ifndef TILE_H
#define TILE_H

#include <QSet>
#include <QPushButton>

class Pawn;
class Player;

class Tile : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(bool safe READ isSafe WRITE setSafe NOTIFY safeChanged)

public:
    explicit Tile(QWidget *parent = nullptr);
    virtual ~Tile();

    bool isSafe() const { return m_safe; }
    const QList<Pawn*>& pawns() const { return m_pawns; }
    Pawn* first() const;
    Pawn* pawnByPlayer(Player* player) const;

public slots:
    void setSafe(bool safe);
    void reset();
    void insertPawn(Pawn* pawn);

signals:
    void safeChanged(bool safe);
    void pawnInserted(Pawn* pawn);
    void pawnRemoved(Pawn* pawn);

private:
    bool m_safe;
    QList<Pawn*> m_pawns;

private slots:
    void handlePawnRemoved(Tile* tile);
    void updateIcon(Pawn* pawn = nullptr);
    void bringForward();

};

#endif // TILE_H

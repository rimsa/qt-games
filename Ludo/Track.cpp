#include "Track.h"
#include "Tile.h"
#include "Pawn.h"
#include "Player.h"

Track::Track(Player* player, QObject *parent)
        : QObject(parent), m_player(player) {

    m_player->m_track = this;
    this->reset();
}

Track::~Track() {
}

void Track::addTile(Tile* tile) {
    Q_ASSERT(!m_tiles.contains(tile));

    m_tiles << tile;
    QObject::connect(tile, SIGNAL(clicked(bool)), this, SLOT(handleClick()));
}

bool Track::markPlayablePawns(int roll) {
    bool playable = false;
    foreach (Pawn* pawn, m_pawns.keys()) {
        if (m_pawns[pawn] + roll <= m_tiles.count()) {
            pawn->setPlayable(true);
            playable = true;
        }
    }

    return playable;
}

bool Track::advance(Pawn* pawn, int steps) {
    Q_ASSERT(pawn != nullptr && pawn->isPlayable());
    Q_ASSERT(m_pawns.contains(pawn));
    Q_ASSERT(steps > 0);

    int total = m_tiles.count();
    int pos = (m_pawns[pawn] += steps);

    if (pos == total) {
        emit finished(pawn);
        return true;
    } else {
        Q_ASSERT(pos < total);

        Tile* tile = m_tiles.at(pos);

        if (!tile->isSafe()) {
            foreach (Pawn* pawn, tile->pawns()) {
                if (pawn->player() != this->player()) {
                    pawn->setOwner(nullptr);
                    emit pawnReturned(pawn);
                }
            }
        }

        tile->insertPawn(pawn);

        return false;
    }
}

void Track::insertPawn(Pawn* pawn) {
    Q_ASSERT(!m_pawns.contains(pawn));
    Q_ASSERT(pawn->player() == this->player());

    m_pawns[pawn] = 0;
    m_tiles.first()->insertPawn(pawn);
}

void Track::removePawn(Pawn* pawn) {
    bool removed = m_pawns.remove(pawn);
    Q_ASSERT(removed);

    Tile* tile = pawn->owner();
    if (tile != nullptr) {
        Q_ASSERT(m_tiles.contains(tile));
        pawn->setOwner(nullptr);
    }
}

void Track::reset() {
    foreach (Pawn* pawn, m_pawns.keys())
        pawn->setOwner(nullptr);

    m_pawns.clear();

    foreach (Tile* tile, m_tiles)
        tile->reset();
}

void Track::handleClick() {
    Tile* tile = qobject_cast<Tile*>(QObject::sender());
    Q_ASSERT(tile != nullptr);

    Pawn* pawn = tile->pawnByPlayer(this->player());
    if (pawn != nullptr && pawn->isPlayable())
        emit pawnSelected(pawn);
}

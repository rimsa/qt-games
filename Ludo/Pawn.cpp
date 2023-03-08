#include "Pawn.h"
#include "Player.h"
#include "Tile.h"

Pawn::Pawn(Player* player)
        : QObject(player),
          m_player(player), m_owner(nullptr) {
}

Pawn::~Pawn() {
}

const QPixmap& Pawn::pixmap() const {
    return m_player->m_pixmap[m_playable ? 1 : 0];
}

void Pawn::setPlayable(bool playable) {
    if (m_playable != playable) {
        m_playable = playable;
        emit playableChanged(playable);
    }
}

void Pawn::setOwner(Tile* owner) {
    if (m_owner != owner) {
        if (m_owner != nullptr)
            emit pawnRemovedFrom(m_owner);

        m_owner = owner;
        if (m_owner != nullptr)
            emit pawnInsertedTo(m_owner);
    }
}

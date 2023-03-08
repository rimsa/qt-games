#include "Tile.h"
#include "Pawn.h"

Tile::Tile(QWidget *parent)
        : QPushButton(parent), m_safe(false) {
}

Tile::~Tile() {
}

Pawn* Tile::first() const {
    return (m_pawns.isEmpty() ? nullptr : m_pawns.first());
}

Pawn* Tile::pawnByPlayer(Player* player) const {
    foreach (Pawn* pawn, m_pawns)
        if (pawn->player() == player)
            return pawn;

    return nullptr;
}

void Tile::setSafe(bool safe) {
    if (m_safe != safe) {
        m_safe = safe;
        emit safeChanged(safe);
    }
}

void Tile::reset() {
    foreach (Pawn* pawn, m_pawns)
        pawn->setOwner(nullptr);

    m_pawns.clear();
}

void Tile::insertPawn(Pawn* pawn) {
    Q_ASSERT(!m_pawns.contains(pawn));

    m_pawns << pawn;

    pawn->setOwner(this);
    QObject::connect(pawn, SIGNAL(pawnRemovedFrom(Tile*)), this, SLOT(handlePawnRemoved(Tile*)));
    QObject::connect(pawn, SIGNAL(playableChanged(bool)), this, SLOT(bringForward()));
    QObject::connect(pawn, SIGNAL(raised()), this, SLOT(bringForward()));

    this->updateIcon(pawn);
    emit pawnInserted(pawn);
}

void Tile::handlePawnRemoved(Tile* tile) {
    Q_ASSERT(tile == this);

    Pawn* pawn = qobject_cast<Pawn*>(QObject::sender());
    Q_ASSERT(pawn != nullptr);

    bool removed = m_pawns.removeOne(pawn);
    Q_ASSERT(removed);

    QObject::disconnect(pawn, SIGNAL(pawnRemovedFrom(Tile*)), this, SLOT(handlePawnRemoved(Tile*)));
    QObject::disconnect(pawn, SIGNAL(playableChanged(bool)), this, SLOT(bringForward()));
    QObject::disconnect(pawn, SIGNAL(raised()), this, SLOT(bringForward()));

    this->updateIcon();
    emit pawnRemoved(pawn);
}

void Tile::updateIcon(Pawn* pawn) {
    if (pawn == nullptr)
        pawn = this->first();

    this->setIcon(pawn ? pawn->pixmap() : QPixmap());
}

void Tile::bringForward() {
    Pawn* pawn = qobject_cast<Pawn*>(QObject::sender());
    Q_ASSERT(m_pawns.contains(pawn));

    this->updateIcon(pawn);
}

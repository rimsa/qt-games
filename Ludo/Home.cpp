#include "Home.h"
#include "ui_Home.h"
#include "Pawn.h"
#include "Player.h"

Home::Home(QWidget *parent)
        : QWidget(parent), ui(new Ui::Home),
          m_player(Player::player(Player::Empty)) {

    ui->setupUi(this);

    for (int idx = 0; idx < 4; idx++) {
        Tile* tile = this->findChild<Tile*>(QString("home%1").arg(idx));
        Q_ASSERT(tile != nullptr);

        m_tiles[idx] = tile;
        QObject::connect(tile, SIGNAL(clicked(bool)), this, SLOT(handleClick()));
    }
}

Home::~Home() {
    delete ui;
}

bool Home::markPlayablePawns(int roll) {
    if (roll != 6 || m_pawns.isEmpty())
        return false;

    foreach (Pawn* pawn, m_pawns)
        pawn->setPlayable(true);

    return true;
}

void Home::setType(Player::Type type) {
    if (type != this->type()) {
        m_player = Player::player(type);
        m_player->m_home = this;
        emit typeChanged(type);
    }
}

void Home::insertPawn(Pawn* pawn) {
    Q_ASSERT(!m_pawns.contains(pawn));
    Q_ASSERT(pawn->player() == this->player());

    for (int idx = 0; idx < 4; idx++) {
        Tile* tile = m_tiles[idx];
        if (tile->first() == nullptr) {
            m_pawns.insert(pawn);
            tile->insertPawn(pawn);
            return;
        }
    }

    Q_UNREACHABLE();
}

void Home::removePawn(Pawn* pawn) {
    bool removed = m_pawns.remove(pawn);
    Q_ASSERT(removed);

    Tile* tile = pawn->owner();
    if (tile != nullptr) {
        bool found = false;
        for (int idx = 0; idx < 4; idx++) {
            if (m_tiles[idx] == tile) {
                found = true;
                break;
            }
        }

        Q_ASSERT(found);
        pawn->setOwner(nullptr);
    }
}

void Home::reset() {
    foreach (Pawn* pawn, m_pawns)
        pawn->setOwner(nullptr);

    m_pawns.clear();

    for (int idx = 0; idx < 4; idx++)
        m_tiles[idx]->reset();
}

void Home::handleClick() {
    Tile* tile = qobject_cast<Tile*>(QObject::sender());
    Q_ASSERT(tile != nullptr);

    Pawn* pawn = tile->first();
    if (pawn != nullptr && pawn->isPlayable())
        emit pawnSelected(pawn);
}

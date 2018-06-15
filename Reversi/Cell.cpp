#include "Cell.h"
#include "Player.h"

Cell::Cell(QWidget *parent)
    : QPushButton(parent),
      m_row(0), m_col(0), m_owner(0) {
}

Cell::~Cell() {
}

void Cell::reset() {
    if (m_owner)
        m_owner->dec();

    m_owner = 0;
    this->setIcon(QPixmap());
}

void Cell::setOwner(Player* player) {
    Q_ASSERT(player != 0);
    Q_ASSERT(m_owner != player);

    if (m_owner)
        m_owner->dec();

    m_owner = player;
    m_owner->inc();

    this->setIcon(QIcon(m_owner->pixmap()));
}

void Cell::flip() {
    Q_ASSERT(m_owner != 0);

    this->setOwner(m_owner->other());
}

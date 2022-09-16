#include "Cell.h"
#include "Player.h"

Cell::Cell(QWidget *parent)
    : QPushButton(parent),
        m_player(nullptr),
        m_inverted(false) {
    QObject::connect(this, SIGNAL(playerChanged(Player*)), this, SLOT(updateCell()));
    QObject::connect(this, SIGNAL(invertedChanged(bool)), this, SLOT(updateCell()));

    this->updateCell();
}

Cell::~Cell() {
}

void Cell::setPlayer(Player* player) {
    if (m_player != player) {
        m_player = player;
        emit playerChanged(player);
    }
}

void Cell::setInverted(bool inverted) {
    if (m_inverted != inverted) {
        m_inverted = inverted;
        emit invertedChanged(inverted);
    }
}

void Cell::reset() {
    m_player = nullptr;
    m_inverted = false;

    this->updateCell();
}

void Cell::updateCell() {
    QPixmap img;

    if (m_player != nullptr) {
        img = m_player->pixmap();
        if (m_inverted)
            img = img.transformed(QTransform().rotate(180));
    }

    this->setIcon(img);
}

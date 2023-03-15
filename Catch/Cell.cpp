#include "Cell.h"
#include <algorithm>

Cell::Cell(QWidget *parent)
        : QPushButton(parent),
          m_row(-1), m_col(-1),
          m_state(State::Empty),
          m_player(nullptr) {
    QObject::connect(this, SIGNAL(stateChanged(Cell::State)), this, SLOT(updateCell()));
    this->updateCell();
}

Cell::~Cell() {
}

void Cell::setState(Cell::State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(m_state);
    }
}

void Cell::setPlayer(Player* player) {
    Q_ASSERT(m_player == nullptr);
    Q_ASSERT(player != nullptr);
    Q_ASSERT(m_state == Cell::Empty);

    m_player = player;
    this->setState(State::Captured);

    emit playerChanged(player);
}

void Cell::reset() {
    m_state = State::Empty;
    m_player = nullptr;

    this->updateCell();
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
void Cell::enterEvent(QEvent *event) {
#else
void Cell::enterEvent(QEnterEvent *event) {
#endif
    emit mouseOver(true);
    QPushButton::enterEvent(event);
}

void Cell::leaveEvent(QEvent *event) {
    emit mouseOver(false);
    QPushButton::leaveEvent(event);
}

void Cell::updateCell() {
    // Setup icon if there is a player.
    this->setIcon(m_player ? m_player->pixmap() : QPixmap());

    // Force reloading stylesheet.
    this->setStyleSheet(this->styleSheet());
}

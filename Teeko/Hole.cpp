#include "Hole.h"
#include <algorithm>

Hole::Hole(QWidget *parent)
        : QPushButton(parent),
          m_row(-1), m_col(-1),
          m_state(State::Empty),
          m_player(nullptr) {
    QObject::connect(this, SIGNAL(stateChanged(Hole::State)), this, SLOT(updateHole()));
    this->updateHole();
}

Hole::~Hole() {
}

void Hole::setState(Hole::State state) {
    if (m_state != state) {
        switch (state) {
            case Hole::Empty:
            case Hole::Playable:
                Q_ASSERT(m_player == nullptr);
                break;
            case Hole::Used:
            case Hole::Selected:
                Q_ASSERT(m_player != nullptr);
                break;
            default:
                Q_UNREACHABLE();
        }

        m_state = state;
        emit stateChanged(m_state);
    }
}

void Hole::setPlayer(Player* player) {
    if (m_player != player) {
        m_player = player;
        this->setState(player ? State::Used : State::Empty);

        emit playerChanged(player);
    }
}

void Hole::reset() {
    m_state = State::Empty;
    m_player = nullptr;

    this->updateHole();
}

void Hole::updateHole() {
    QPixmap pixmap;

    switch (m_state) {
        case State::Empty:
            pixmap = QPixmap(":/empty");
            break;
        case State::Playable:
            pixmap = QPixmap(":/playable");
            break;
        case State::Used:
            pixmap = m_player->pixmap();
            break;
        case State::Selected:
            pixmap = m_player->selected();
            break;
        default:
            Q_UNREACHABLE();
    }

    this->setIcon(pixmap);
}

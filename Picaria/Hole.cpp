#include "Hole.h"
#include <algorithm>

Hole::Hole(QWidget *parent)
        : QPushButton(parent),
          m_state(Hole::EmptyState) {
    this->updateHole(m_state);
    std::fill(m_neighbors, m_neighbors + 8, nullptr);

    QObject::connect(this, SIGNAL(stateChanged(State)), this, SLOT(updateHole(State)));
}

Hole::~Hole() {
}

void Hole::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

QList<Hole::Direction> Hole::directions() {
    QList<Hole::Direction> directions;

    directions << Hole::North
        << Hole::NorthEast
        << Hole::East
        << Hole::SouthEast
        << Hole::South
        << Hole::SouthWest
        << Hole::West
        << Hole::NorthWest;

    return directions;
}

void Hole::reset() {
    m_state = Hole::EmptyState;
    std::fill(m_neighbors, m_neighbors + 8, nullptr);
    this->updateHole(m_state);
}

QPixmap Hole::stateToPixmap(State state) {
    switch (state) {
        case Hole::EmptyState:
            return QPixmap(":empty");
        case Hole::RedState:
            return QPixmap(":red");
        case Hole::BlueState:
            return QPixmap(":blue");
        case Hole::SelectableState:
            return QPixmap(":selectable");
        default:
            return QPixmap();
    }
}

void Hole::updateHole(State state) {
    this->setIcon(Hole::stateToPixmap(state));
}

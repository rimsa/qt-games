#include "Hole.h"

Hole::Hole(QWidget *parent)
    : QPushButton(parent),
      m_row(0), m_col(0), m_state(Hole::EmptyState) {
    QObject::connect(this, SIGNAL(stateChanged(Hole::State)),
                     this, SLOT(updateHole(Hole::State)));
}

Hole::~Hole() {
}

void Hole::setState(Hole::State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

void Hole::reset() {
    m_state = Hole::EmptyState;
    this->updateHole(m_state);
}

QPixmap Hole::state2pixmap(Hole::State state) {
    switch (state) {
        case Hole::EmptyState:
            return QPixmap(":/empty");
        case Hole::RedState:
            return QPixmap(":/red");
        case Hole::BlueState:
            return QPixmap(":/blue");
        case Hole::SelectableState:
            return QPixmap(":/selectable");
        default:
            return QPixmap();
    }
}

void Hole::updateHole(Hole::State state) {
    this->setIcon(Hole::state2pixmap(state));
}

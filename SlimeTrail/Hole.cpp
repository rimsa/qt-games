#include "Hole.h"
#include <algorithm>

Hole::Hole(QWidget *parent)
        : QPushButton(parent),
          m_row(-1), m_col(-1),
          m_state(Hole::EmptyState),
          m_marked(false) {
    QObject::connect(this, SIGNAL(stateChanged(Hole::State)), this, SLOT(updateHole()));
    QObject::connect(this, SIGNAL(markedChanged(bool)), this, SLOT(updateHole()));

    this->updateHole();
}

Hole::~Hole() {
}

void Hole::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

void Hole::setMarked(bool marked) {
    if (m_marked != marked) {
        m_marked = marked;
        emit markedChanged(marked);
    }
}

void Hole::reset() {
    m_state = Hole::EmptyState;
    m_marked = false;

    this->updateHole();
}

QPixmap Hole::stateToPixmap(State state) {
    switch (state) {
        case Hole::WhiteState:
            return QPixmap(":/white");
        case Hole::BlackState:
            return QPixmap(":/black");
        case Hole::EmptyState:
        default:
            return QPixmap();
    }
}

void Hole::updateHole() {
    // Setup icon.
    this->setIcon(Hole::stateToPixmap(m_state));

    // Force reloading stylesheet.
    this->setStyleSheet(this->styleSheet());
}

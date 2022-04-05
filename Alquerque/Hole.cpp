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

void Hole::updateHole() {
    QIcon icon;
    switch (m_state) {
        case Hole::EmptyState:
            icon.addPixmap(QPixmap(m_marked ? ":/empty-marked" : ":/empty-std"));
            icon.addPixmap(QPixmap(":/empty-std"), QIcon::Disabled);
            break;
        case Hole::RedState:
            icon.addPixmap(QPixmap(m_marked ? ":/red-marked" : ":/red-std"));
            icon.addPixmap(QPixmap(":/red-disabled"), QIcon::Disabled);
            break;
        case Hole::BlueState:
            icon.addPixmap(QPixmap(m_marked ? ":/blue-marked" : ":/blue-std"));
            icon.addPixmap(QPixmap(":/blue-disabled"), QIcon::Disabled);
            break;
        default:
            Q_UNREACHABLE();
            return;
    }

    this->setIcon(icon);
}

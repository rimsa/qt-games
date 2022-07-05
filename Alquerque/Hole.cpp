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

QList<Hole::Direction> Hole::moves() {
    Q_ASSERT(m_row >= 0 && m_row < 5);
    Q_ASSERT(m_col >= 0 && m_col < 5);

    switch (m_row) {
        case 0:
            switch (m_col) {
                case 0:
                    return QList<Hole::Direction>() << Hole::East << Hole::SouthEast << Hole::South;
                case 1:
                    return QList<Hole::Direction>() << Hole::East << Hole::South << Hole::West;
                case 2:
                    return QList<Hole::Direction>() << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West;
                case 3:
                    return QList<Hole::Direction>() << Hole::East << Hole::South << Hole::West;
                case 4:
                    return QList<Hole::Direction>() << Hole::South << Hole::SouthWest << Hole::West;
                default:
                    Q_UNREACHABLE();
            }

            break;
        case 1:
            switch (m_col) {
                case 0:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South;
                case 1:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                case 2:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South << Hole::West;
                case 3:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                case 4:
                    return QList<Hole::Direction>() << Hole::North << Hole::South << Hole::West;
                default:
                    Q_UNREACHABLE();
            }

            break;
        case 2:
            switch (m_col) {
                case 0:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South;
                case 1:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South << Hole::West;
                case 2:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                case 3:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South << Hole::West;
                case 4:
                    return QList<Hole::Direction>() << Hole::North << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                default:
                    Q_UNREACHABLE();
            }

            break;
        case 3:
            switch (m_col) {
                case 0:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South;
                case 1:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                case 2:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::South << Hole::West;
                case 3:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::SouthEast << Hole::South << Hole::SouthWest << Hole::West << Hole::NorthWest;
                case 4:
                    return QList<Hole::Direction>() << Hole::North << Hole::South << Hole::West;
                default:
                    Q_UNREACHABLE();
            }

            break;
        case 4:
            switch (m_col) {
                case 0:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East;
                case 1:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::West;
                case 2:
                    return QList<Hole::Direction>() << Hole::North << Hole::NorthEast << Hole::East << Hole::West << Hole::NorthWest;
                case 3:
                    return QList<Hole::Direction>() << Hole::North << Hole::East << Hole::West;
                case 4:
                    return QList<Hole::Direction>() << Hole::North << Hole::West << Hole::NorthWest;
                default:
                    Q_UNREACHABLE();
            }

            break;
        default:
            Q_UNREACHABLE();
    }

    return QList<Hole::Direction>();
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

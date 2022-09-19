#include "Lane.h"
#include "Cell.h"
#include "Player.h"
#include "Squadro.h"

#define LANE_SIZE 7

Lane::Lane(Player* player, const QList<Cell*>& cells,
        int going, int returning, QObject *parent)
    : QObject(parent), m_player(player), m_cells(cells),
        m_steps{going, returning}, m_direction(Lane::Forward), m_current(0) {

    Q_ASSERT(cells.size() == LANE_SIZE);
    this->reset();
}

Lane::~Lane() {
}

Cell* Lane::home() const {
    return m_cells.at(0);
}

Cell* Lane::end() const {
    return m_cells.at(LANE_SIZE - 1);
}

Cell* Lane::current() const {
    Q_ASSERT(m_current >= 0 && m_current < m_cells.size());
    return m_cells.at(m_current);
}

bool Lane::contains(Cell* cell) {
    return m_cells.contains(cell);
}

bool Lane::isFinished() {
    return m_current == 0 && m_direction == Lane::Backward;
}

void Lane::reset() {
    m_current = 0;
    m_direction = Lane::Forward;

    foreach (Cell* cell, m_cells) {
        cell->reset();
        cell->setEnabled(true);
    }

    this->home()->setPlayer(m_player);
}

void Lane::move() {
    Q_ASSERT(!isFinished());

    Cell* cell = this->current();
    Q_ASSERT(cell != nullptr && cell->player() == m_player);
    cell->setPlayer(nullptr);

    int moves;
    int step;
    Cell* wall;
    if (m_direction == Lane::Forward) {
        moves = m_steps.forward;
        step = 1;
        wall = this->end();
    } else {
        moves = m_steps.backward;
        step = -1;
        wall = this->home();
    }

    Player* other = m_player->other();
    while (moves > 0) {
        m_current += step;
        cell = this->current();
        if (cell->player() == other || cell == wall)
            break;

        moves--;
    }

    while (cell->player() == other) {
        Lane* lane = other->findLane(cell);
        Q_ASSERT(lane != nullptr);
        lane->sendBack();

        m_current += step;
        cell = this->current();
    }

    Q_ASSERT(cell->player() == nullptr);
    cell->setPlayer(m_player);
    if (m_direction == Lane::Forward) {
        if (cell == wall) {
            m_direction = Lane::Backward;
            cell->setInverted(true);
        } else {
            cell->setInverted(false);
        }
    } else {
        cell->setInverted(true);
        if (cell == wall) {
            cell->setEnabled(false);
            emit finished();
        }
    }
}

void Lane::sendBack() {
    Cell* cell = this->current();
    Q_ASSERT(cell->player() == m_player);
    cell->setPlayer(nullptr);

    bool forward = m_direction == Lane::Forward;
    m_current = (forward ? 0 : (LANE_SIZE - 1));
    cell = this->current();
    cell->setPlayer(m_player);
    cell->setInverted(!forward);
}

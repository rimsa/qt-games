#include "Achi.h"
#include "ui_Achi.h"

#include <QMessageBox>
#include <QSignalMapper>

Achi::Player state2player(Hole::State state) {
    switch (state) {
        case Hole::RedState:
            return Achi::RedPlayer;
        case Hole::BlueState:
            return Achi::BluePlayer;
        default:
            Q_UNREACHABLE();
    }
}

Hole::State player2state(Achi::Player player) {
    return player == Achi::RedPlayer ? Hole::RedState : Hole::BlueState;
}

Achi::Achi(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Achi),
      m_player(Achi::RedPlayer),
      m_phase(Achi::DropPhase),
      m_dropCount(0),
      m_selected(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(showGameOver(Player)));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(reset()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 9; ++id) {
        int r = id / 3;
        int c = id % 3;

        Hole* hole = this->findChild<Hole*>(QString("hole%1%2").arg(r).arg(c));
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));
    }
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));

    this->updateStatusBar();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Achi::~Achi() {
    delete ui;
}

Hole* Achi::holeAt(int row, int col) const {
    if (row >= 0 && row < 3 &&
        col >= 0 && col < 3) {
        int index = row * 3 + col;
        return m_holes[index];
    } else {
        return nullptr;
    }
}

void Achi::play(int id) {
    Hole* hole = m_holes[id];
    Q_ASSERT(hole != nullptr);

    switch (m_phase) {
        case Achi::DropPhase:
            drop(hole);
            break;
        case Achi::MovePhase:
            move(hole);
            break;
        default:
            Q_UNREACHABLE();
    }
}

void Achi::drop(Hole* hole) {
    if (hole->state() == Hole::EmptyState) {
        hole->setState(player2state(m_player));

        if (isGameOver(hole))
            emit gameOver(m_player);
        else {
            ++m_dropCount;
            if (m_dropCount == 6)
                m_phase = Achi::MovePhase;

            this->switchPlayer();
        }
    }
}

void Achi::move(Hole* hole) {
    QPair<Hole*,Hole*>* movement = nullptr;

    if (hole->state() == Hole::SelectableState) {
        Q_ASSERT(m_selected != 0);
        movement = new QPair<Hole*,Hole*>(m_selected, hole);
    } else {
        if (hole->state() == player2state(m_player)) {
            QList<Hole*> selectables = this->findSelectables(hole);
            if (selectables.count() == 1) {
                movement = new QPair<Hole*,Hole*>(hole, selectables.at(0));
            } else if (selectables.count() > 1) {
                this->clearSelectable();
                foreach (Hole* tmp, selectables)
                    tmp->setState(Hole::SelectableState);

                m_selected = hole;
            }
        }
    }

    if (movement != nullptr) {
        this->clearSelectable();
        m_selected = 0;

        Q_ASSERT(movement->first->state() == player2state(m_player));
        Q_ASSERT(movement->second->state() == Hole::EmptyState);

        movement->first->setState(Hole::EmptyState);
        movement->second->setState(player2state(m_player));

        if (isGameOver(movement->second))
            emit gameOver(m_player);
        else
            this->switchPlayer();

        delete movement;
    }
}

void Achi::switchPlayer() {
    m_player = m_player == Achi::RedPlayer ?
                    Achi::BluePlayer : Achi::RedPlayer;
    this->updateStatusBar();
}

bool isSelectable(Hole* hole) {
    return hole != nullptr &&
            (hole->state() == Hole::EmptyState ||
             hole->state() == Hole::SelectableState);
}

void Achi::clearSelectable() {
    for (int id = 0; id < 9; id++) {
        Hole* hole = m_holes[id];
        if (hole->state() == Hole::SelectableState)
            hole->setState(Hole::EmptyState);
    }
}

QList<Hole*> Achi::findSelectables(Hole* hole) {
    QList<Hole*> list;
    int id = hole->row() * 3 + hole->col();

    Hole* east = this->holeAt(hole->row() - 1, hole->col());
    if (isSelectable(east))
        list << east;

    if (id == 4 || id == 8) {
        Hole* north_east = this->holeAt(hole->row() - 1, hole->col() - 1);
        if (isSelectable(north_east))
            list << north_east;
    }

    Hole* north = this->holeAt(hole->row(), hole->col() - 1);
    if (isSelectable(north))
        list << north;

    if (id == 4 || id == 6) {
        Hole* north_west = this->holeAt(hole->row() + 1, hole->col() - 1);
        if (isSelectable(north_west))
            list << north_west;
    }

    Hole* west = this->holeAt(hole->row() + 1, hole->col());
    if (isSelectable(west))
        list << west;

    if (id == 0 || id == 4) {
        Hole* south_west = this->holeAt(hole->row() + 1, hole->col() + 1);
        if (isSelectable(south_west))
            list << south_west;
    }

    Hole* south = this->holeAt(hole->row(), hole->col() + 1);
    if (isSelectable(south))
        list << south;

    if (id == 2 || id == 4) {
        Hole* south_east = this->holeAt(hole->row() - 1, hole->col() + 1);
        if (isSelectable(south_east))
            list << south_east;
    }

    return list;
}

bool Achi::checkRow(Player player, int col) {
    Hole::State state = player2state(player);
    for (int r = 0; r < 3; ++r) {
        Hole* tmp = this->holeAt(r, col);
        Q_ASSERT(tmp != 0);

        switch (tmp->state()) {
            case Hole::RedState:
            case Hole::BlueState:
                if (state != tmp->state())
                    return false;

                break;
            default:
                return false;
        }
    }

    return true;
}

bool Achi::checkCol(Player player, int row) {
    Hole::State state = player2state(player);
    for (int c = 0; c < 3; ++c) {
        Hole* tmp = this->holeAt(row, c);
        Q_ASSERT(tmp != 0);

        switch (tmp->state()) {
            case Hole::RedState:
            case Hole::BlueState:
                if (state != tmp->state())
                    return false;

                break;
            default:
                return false;
        }
    }

    return true;
}

bool Achi::checkDiagonal(Player player) {
    Hole::State state = player2state(player);
    for (int n = 0; n < 3; ++n) {
        Hole* tmp = this->holeAt(n, n);
        Q_ASSERT(tmp != nullptr);

        switch (tmp->state()) {
            case Hole::RedState:
            case Hole::BlueState:
                if (state != tmp->state())
                    return false;

                break;
            default:
                return false;
        }
    }

    return true;
}

bool Achi::checkCrossDiagonal(Player player) {
    Hole::State state = player2state(player);
    for (int r = 0, c = 2; r < 3; ++r, --c) {
        Hole* tmp = this->holeAt(r, c);
        Q_ASSERT(tmp != nullptr);

        switch (tmp->state()) {
            case Hole::RedState:
            case Hole::BlueState:
                if (state != tmp->state())
                    return false;

                break;
            default:
                return false;
        }
    }

    return true;
}

bool Achi::isGameOver(Hole* hole) {
    Achi::Player player = state2player(hole->state());
    return this->checkRow(player, hole->col()) || this->checkCol(player, hole->row()) ||
           this->checkDiagonal(player) || this->checkCrossDiagonal(player);
}

void Achi::reset() {
    for (int id = 0; id < 9; ++id) {
        Hole* hole = m_holes[id];
        Q_ASSERT(hole != nullptr);

        m_holes[id]->reset();
    }

    m_player = Achi::RedPlayer;
    m_phase = Achi::DropPhase;
    m_dropCount = 0;
    m_selected = nullptr;

    this->updateStatusBar();
}

void Achi::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Achi\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Achi::showGameOver(Player player) {
    switch (player) {
        case Achi::RedPlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador vermelho venceu."));
            break;
        case Achi::BluePlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador azul venceu."));
            break;
        default:
            Q_UNREACHABLE();
    }
}

void Achi::updateStatusBar() {
    QString player(m_player == Achi::RedPlayer ? "vermelho" : "azul");
    QString phase(m_phase == Achi::DropPhase ? "colocar" : "mover");

    ui->statusbar->showMessage(tr("Fase de %1: vez do jogador %2").arg(phase).arg(player));
}

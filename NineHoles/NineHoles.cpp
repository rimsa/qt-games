#include "NineHoles.h"
#include "ui_NineHoles.h"

#include <QDebug>
#include <QSignalMapper>
#include <QMessageBox>

NineHoles::Player state2player(Hole::State state) {
    switch (state) {
        case Hole::RedState:
            return NineHoles::RedPlayer;
        case Hole::BlueState:
            return NineHoles::BluePlayer;
        default:
            Q_UNREACHABLE();
    }
}

Hole::State player2state(NineHoles::Player player) {
    return player == NineHoles::RedPlayer ? Hole::RedState : Hole::BlueState;
}

NineHoles::NineHoles(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::NineHoles),
      m_player(NineHoles::RedPlayer),
      m_phase(NineHoles::DropPhase),
      m_dropCount(0),
      m_selected(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(showGameOver(Player)));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(reset()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 9; id++) {
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

NineHoles::~NineHoles() {
    delete ui;
}

Hole* NineHoles::holeAt(int row, int col) const {
    if (row >= 0 && row < 3 &&
        col >= 0 && col < 3) {
        int index = row * 3 + col;
        return m_holes[index];
    } else {
        return 0;
    }
}

void NineHoles::play(int id) {
    Hole* hole = m_holes[id];

    switch (m_phase) {
        case NineHoles::DropPhase:
            drop(hole);
            break;
        case NineHoles::MovePhase:
            move(hole);
            break;
        default:
            Q_UNREACHABLE();
    }
}

void NineHoles::drop(Hole* hole) {
    if (hole->state() == Hole::EmptyState) {
        hole->setState(player2state(m_player));

        if (isGameOver(hole))
            emit gameOver(m_player);
        else {
            ++m_dropCount;
            if (m_dropCount == 6)
                m_phase = NineHoles::MovePhase;

            this->switchPlayer();
        }
    }
}

void NineHoles::move(Hole* hole) {
    QPair<Hole*,Hole*>* movement = nullptr;

    if (hole->state() == Hole::SelectableState) {
        Q_ASSERT(m_selected != 0);
        movement = new QPair<Hole*,Hole*>(m_selected, hole);
    } else {
        if (hole->state() == player2state(m_player)) {
            QList<Hole*> selectable = this->findSelectable(hole);
            if (selectable.count() == 1) {
                movement = new QPair<Hole*,Hole*>(hole, selectable.at(0));
            } else if (selectable.count() > 1) {
                this->clearSelectable();
                foreach (Hole* tmp, selectable)
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

void NineHoles::switchPlayer() {
    m_player = m_player == NineHoles::RedPlayer ?
                    NineHoles::BluePlayer : NineHoles::RedPlayer;
    this->updateStatusBar();
}

bool isSelectable(Hole* hole) {
    return hole != nullptr &&
            (hole->state() == Hole::EmptyState ||
             hole->state() == Hole::SelectableState);
}

void NineHoles::clearSelectable() {
    for (int id = 0; id < 9; id++) {
        Hole* hole = m_holes[id];
        if (hole->state() == Hole::SelectableState)
            hole->setState(Hole::EmptyState);
    }
}

QList<Hole*> NineHoles::findSelectable(Hole* hole) {
    QList<Hole*> list;

    Hole* left = this->holeAt(hole->row() - 1, hole->col());
    if (isSelectable(left))
        list << left;

    Hole* up = this->holeAt(hole->row(), hole->col() - 1);
    if (isSelectable(up))
        list << up;

    Hole* right = this->holeAt(hole->row() + 1, hole->col());
    if (isSelectable(right))
        list << right;

    Hole* bottom = this->holeAt(hole->row(), hole->col() + 1);
    if (isSelectable(bottom))
        list << bottom;

    return list;
}

bool NineHoles::checkRow(Player player, int col) {
    Hole::State state = player2state(player);
    for (int r = 0; r < 3; r++) {
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

bool NineHoles::checkCol(Player player, int row) {
    Hole::State state = player2state(player);
    for (int c = 0; c < 3; c++) {
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

bool NineHoles::isGameOver(Hole* hole) {
    NineHoles::Player player = state2player(hole->state());
    return this->checkRow(player, hole->col()) || this->checkCol(player, hole->row());
}

void NineHoles::reset() {
    for (int id = 0; id < 9; id++) {
        Hole* hole = m_holes[id];
        hole->reset();
    }

    m_player = NineHoles::RedPlayer;
    m_phase = NineHoles::DropPhase;
    m_dropCount = 0;
    m_selected = nullptr;

    this->updateStatusBar();
}

void NineHoles::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Nine Holes\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void NineHoles::showGameOver(Player player) {
    switch (player) {
        case NineHoles::RedPlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador vermelho venceu."));
            break;
        case NineHoles::BluePlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador azul venceu."));
            break;
        default:
            Q_UNREACHABLE();
    }
}

void NineHoles::updateStatusBar() {
    QString player(m_player == NineHoles::RedPlayer ? "vermelho" : "azul");
    QString phase(m_phase == NineHoles::DropPhase ? "colocar" : "mover");

    ui->statusbar->showMessage(tr("Fase de %1: vez do jogador %2").arg(phase).arg(player));
}

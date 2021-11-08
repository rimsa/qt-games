#include "Picaria.h"
#include "ui_Picaria.h"

#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

Picaria::Player state2player(Hole::State state) {
    switch (state) {
        case Hole::RedState:
            return Picaria::RedPlayer;
        case Hole::BlueState:
            return Picaria::BluePlayer;
        default:
            Q_UNREACHABLE();
    }
}

Hole::State player2state(Picaria::Player player) {
    return player == Picaria::RedPlayer ? Hole::RedState : Hole::BlueState;
}

bool checkWin(Hole* h1, Hole* h2, Hole* h3) {
    if (h1 != nullptr && h2 != nullptr && h3 != nullptr) {
        Hole::State state = h1->state();
        if (state == Hole::RedState || state == Hole::BlueState)
            return h2->state() == state && h3->state() == state;
    }

    return false;
}

Picaria::Picaria(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Picaria),
      m_mode(Picaria::NineHoles),
      m_player(Picaria::RedPlayer),
      m_phase(Picaria::DropPhase),
      m_dropCount(0),
      m_selected(nullptr) {

    ui->setupUi(this);

    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    modeGroup->addAction(ui->action9holes);
    modeGroup->addAction(ui->action13holes);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(modeGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateMode(QAction*)));
    QObject::connect(this, SIGNAL(modeChanged(Picaria::Mode)), this, SLOT(reset()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    QObject::connect(this, SIGNAL(gameOver(Picaria::Player)), this, SLOT(showGameOver(Picaria::Player)));
    QObject::connect(this, SIGNAL(gameOver(Picaria::Player)), this, SLOT(reset()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 13; ++id) {
        QString holeName = QString("hole%1").arg(id+1, 2, 10, QChar('0'));
        Hole* hole = this->findChild<Hole*>(holeName);
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Picaria::~Picaria() {
    delete ui;
}

void Picaria::setMode(Picaria::Mode mode) {
    if (m_mode != mode) {
        m_mode = mode;
        emit modeChanged(mode);
    }
}

void Picaria::switchPlayer() {
    m_player = m_player == Picaria::RedPlayer ?
                    Picaria::BluePlayer : Picaria::RedPlayer;
    this->updateStatusBar();
}

QList<Hole*> Picaria::findSelectables(Hole* hole) {
    QList<Hole*> holes;

    foreach (Hole::Direction direction, Hole::directions()) {
        Hole* neighbor = hole->neighbor(direction);
        if (neighbor != nullptr) {
            switch (neighbor->state()) {
                case Hole::EmptyState:
                case Hole::SelectableState:
                    holes << neighbor;
                    break;
                default:
                    break;
            }
        }
    }

    return holes;
}

bool Picaria::isGameOver(Hole* hole) {
    // On vertical.
    {
        Hole* north1 = hole->neighbor(Hole::North);
        Hole* north2 = north1 != nullptr ? north1->neighbor(Hole::North) : nullptr;

        Hole* south1 = hole->neighbor(Hole::South);
        Hole* south2 = south1 != nullptr ? south1->neighbor(Hole::South) : nullptr;

        if (checkWin(north2, north1, hole) ||
                checkWin(north1, hole, south1) ||
                checkWin(hole, south1, south2))
            return true;
    }

    // On horizontal.
    {
        Hole* east1 = hole->neighbor(Hole::East);
        Hole* east2 = east1 != nullptr ? east1->neighbor(Hole::East) : nullptr;

        Hole* west1 = hole->neighbor(Hole::West);
        Hole* west2 = west1 != nullptr ? west1->neighbor(Hole::West) : nullptr;

        if (checkWin(west2, west1, hole) ||
                checkWin(west1, hole, east1) ||
                checkWin(hole, east1, east2))
            return true;
    }

    // On diagonal.
    {
        Hole* northWest1 = hole->neighbor(Hole::NorthWest);
        Hole* northWest2 = northWest1 != nullptr ? northWest1->neighbor(Hole::NorthWest) : nullptr;

        Hole* southEast1 = hole->neighbor(Hole::SouthEast);
        Hole* southEast2 = southEast1 != nullptr ? southEast1->neighbor(Hole::SouthEast) : nullptr;

        if (checkWin(northWest2, northWest1, hole) ||
                checkWin(northWest1, hole, southEast1) ||
                checkWin(hole, southEast1, southEast2))
            return true;
    }

    // On cross diagonal.
    {
        Hole* northEast1 = hole->neighbor(Hole::NorthEast);
        Hole* northEast2 = northEast1 != nullptr ? northEast1->neighbor(Hole::NorthEast) : nullptr;

        Hole* southWest1 = hole->neighbor(Hole::SouthWest);
        Hole* southWest2 = southWest1 != nullptr ? southWest1->neighbor(Hole::SouthWest) : nullptr;

        if (checkWin(northEast2, northEast1, hole) ||
                checkWin(northEast1, hole, southWest1) ||
                checkWin(hole, southWest1, southWest2))
            return true;
    }

    return false;
}

void Picaria::play(int id) {
    switch (m_phase) {
        case Picaria::DropPhase:
            drop(id);
            break;
        case Picaria::MovePhase:
            move(id);
            break;
        default:
            Q_UNREACHABLE();
    }
}

void Picaria::drop(int id) {
    Hole* hole = m_holes[id];
    if (hole->state() == Hole::EmptyState) {
        hole->setState(player2state(m_player));

        if (isGameOver(hole))
            emit gameOver(m_player);
        else {
            ++m_dropCount;
            if (m_dropCount == 6)
                m_phase = Picaria::MovePhase;

            this->switchPlayer();
        }
    }
}

void Picaria::move(int id) {
    Hole* hole = m_holes[id];

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
                this->clearSelectables();
                foreach (Hole* tmp, selectables)
                    tmp->setState(Hole::SelectableState);

                m_selected = hole;
            }
        }
    }

    if (movement != nullptr) {
        this->clearSelectables();
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

void Picaria::reset() {
    // Reset each hole.
    for (int id = 0; id < 13; ++id) {
        Hole* hole = m_holes[id];
        hole->reset();

        // Set the hole nighbors and visibility according to the board mode.
        switch (id) {
            case 0:
                hole->setNeighbor(Hole::East, m_holes[1]);
                hole->setNeighbor(Hole::SouthEast, m_mode == Picaria::ThirteenHoles ? m_holes[3] : m_holes[6]);
                hole->setNeighbor(Hole::South, m_holes[5]);
                break;
            case 1:
                hole->setNeighbor(Hole::East, m_holes[2]);
                hole->setNeighbor(Hole::SouthEast, m_mode == Picaria::ThirteenHoles ? m_holes[4] : m_holes[7]);
                hole->setNeighbor(Hole::South, m_holes[6]);
                hole->setNeighbor(Hole::SouthWest, m_mode == Picaria::ThirteenHoles ? m_holes[3] : m_holes[5]);
                hole->setNeighbor(Hole::West, m_holes[0]);
                break;
            case 2:
                hole->setNeighbor(Hole::South, m_holes[7]);
                hole->setNeighbor(Hole::SouthWest, m_mode == Picaria::ThirteenHoles ? m_holes[4] : m_holes[6]);
                hole->setNeighbor(Hole::West, m_holes[1]);
                break;
            case 3:
                if (m_mode == Picaria::ThirteenHoles) {
                    hole->setNeighbor(Hole::NorthEast, m_holes[1]);
                    hole->setNeighbor(Hole::SouthEast, m_holes[6]);
                    hole->setNeighbor(Hole::SouthWest, m_holes[5]);
                    hole->setNeighbor(Hole::NorthWest, m_holes[0]);
                    hole->show();
                } else {
                    hole->hide();
                }

                break;
            case 4:
                if (m_mode == Picaria::ThirteenHoles) {
                    hole->setNeighbor(Hole::NorthEast, m_holes[2]);
                    hole->setNeighbor(Hole::SouthEast, m_holes[7]);
                    hole->setNeighbor(Hole::SouthWest, m_holes[6]);
                    hole->setNeighbor(Hole::NorthWest, m_holes[1]);
                    hole->show();
                } else {
                    hole->hide();
                }

                break;
            case 5:
                hole->setNeighbor(Hole::North, m_holes[0]);
                hole->setNeighbor(Hole::NorthEast, m_mode == Picaria::ThirteenHoles ? m_holes[3] : m_holes[1]);
                hole->setNeighbor(Hole::East, m_holes[6]);
                hole->setNeighbor(Hole::SouthEast, m_mode == Picaria::ThirteenHoles ? m_holes[8] : m_holes[11]);
                hole->setNeighbor(Hole::South, m_holes[10]);
                break;
            case 6:
                hole->setNeighbor(Hole::North, m_holes[1]);
                hole->setNeighbor(Hole::NorthEast, m_mode == Picaria::ThirteenHoles ? m_holes[4] : m_holes[2]);
                hole->setNeighbor(Hole::East, m_holes[7]);
                hole->setNeighbor(Hole::SouthEast, m_mode == Picaria::ThirteenHoles ? m_holes[9] : m_holes[12]);
                hole->setNeighbor(Hole::South, m_holes[11]);
                hole->setNeighbor(Hole::SouthWest, m_mode == Picaria::ThirteenHoles ? m_holes[8] : m_holes[10]);
                hole->setNeighbor(Hole::West, m_holes[5]);
                hole->setNeighbor(Hole::NorthWest, m_mode == Picaria::ThirteenHoles ? m_holes[3] : m_holes[0]);
                break;
            case 7:
                hole->setNeighbor(Hole::North, m_holes[2]);
                hole->setNeighbor(Hole::South, m_holes[12]);
                hole->setNeighbor(Hole::SouthWest, m_mode == Picaria::ThirteenHoles ? m_holes[9] : m_holes[11]);
                hole->setNeighbor(Hole::West, m_holes[6]);
                hole->setNeighbor(Hole::NorthWest, m_mode == Picaria::ThirteenHoles ? m_holes[4] : m_holes[1]);
                break;
            case 8:
                if (m_mode == Picaria::ThirteenHoles) {
                    hole->setNeighbor(Hole::NorthEast, m_holes[6]);
                    hole->setNeighbor(Hole::SouthEast, m_holes[11]);
                    hole->setNeighbor(Hole::SouthWest, m_holes[10]);
                    hole->setNeighbor(Hole::NorthWest, m_holes[5]);
                    hole->show();
                } else {
                    hole->hide();
                }

                break;
            case 9:
                if (m_mode == Picaria::ThirteenHoles) {
                    hole->setNeighbor(Hole::NorthEast, m_holes[7]);
                    hole->setNeighbor(Hole::SouthEast, m_holes[12]);
                    hole->setNeighbor(Hole::SouthWest, m_holes[11]);
                    hole->setNeighbor(Hole::NorthWest, m_holes[6]);
                    hole->show();
                } else {
                    hole->hide();
                }

                break;
            case 10:
                hole->setNeighbor(Hole::North, m_holes[5]);
                hole->setNeighbor(Hole::NorthEast, m_mode == Picaria::ThirteenHoles ? m_holes[8] : m_holes[6]);
                hole->setNeighbor(Hole::East, m_holes[11]);
                break;
            case 11:
                hole->setNeighbor(Hole::North, m_holes[6]);
                hole->setNeighbor(Hole::NorthEast, m_mode == Picaria::ThirteenHoles ? m_holes[9] : m_holes[7]);
                hole->setNeighbor(Hole::East, m_holes[12]);
                hole->setNeighbor(Hole::West, m_holes[10]);
                hole->setNeighbor(Hole::NorthWest, m_mode == Picaria::ThirteenHoles ? m_holes[8] : m_holes[5]);
                break;
            case 12:
                hole->setNeighbor(Hole::North, m_holes[7]);
                hole->setNeighbor(Hole::West, m_holes[11]);
                hole->setNeighbor(Hole::NorthWest, m_mode == Picaria::ThirteenHoles ? m_holes[9] : m_holes[6]);
                break;
            default:
                break;
        }
    }

    // Reset the player, phase, drop count and selected.
    m_player = Picaria::RedPlayer;
    m_phase = Picaria::DropPhase;
    m_dropCount = 0;
    m_selected = nullptr;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Picaria::clearSelectables() {
    for (int id = 0; id < 13; id++) {
        Hole* hole = m_holes[id];
        if (hole->state() == Hole::SelectableState)
            hole->setState(Hole::EmptyState);
    }
}

void Picaria::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Picaria\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Picaria::showGameOver(Picaria::Player player) {
    switch (player) {
        case Picaria::RedPlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador vermelho venceu."));
            break;
        case Picaria::BluePlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador azul venceu."));
            break;
        default:
            Q_UNREACHABLE();
    }
}

void Picaria::updateMode(QAction* action) {
    if (action == ui->action9holes)
        this->setMode(Picaria::NineHoles);
    else if (action == ui->action13holes)
        this->setMode(Picaria::ThirteenHoles);
    else
        Q_UNREACHABLE();
}

void Picaria::updateStatusBar() {
    QString player(m_player == Picaria::RedPlayer ? "vermelho" : "azul");
    QString phase(m_phase == Picaria::DropPhase ? "colocar" : "mover");

    ui->statusbar->showMessage(tr("Fase de %1: vez do jogador %2").arg(phase).arg(player));
}

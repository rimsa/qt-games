#include "Teeko.h"
#include "ui_Teeko.h"

#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

Teeko::Teeko(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Teeko),
      m_player(Player::player(Player::Red)),
      m_phase(Teeko::DropPhase),
      m_dropCount(0),
      m_selected(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            QString holeName = QString("hole%1%2").arg(row).arg(col);
            Hole* hole = this->findChild<Hole*>(holeName);
            Q_ASSERT(hole != nullptr);

            m_board[row][col] = hole;

            int id = row * 5 + col;
            map->setMapping(hole, id);
            QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));

    // Notify when the game is over and reset.
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(showGameOver()));
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(reset()));

    // Reset the game.
    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Teeko::~Teeko() {
    delete ui;
}

void Teeko::setPhase(Teeko::Phase phase) {
    if (m_phase != phase) {
        m_phase = phase;
        emit phaseChanged(phase);
    }
}

void Teeko::play(int id) {
    Hole* hole = m_board[id / 5][id % 5];
    switch (m_phase) {
        case Teeko::DropPhase:
            drop(hole);
            break;
        case Teeko::MovePhase:
            move(hole);
            break;
        default:
            Q_UNREACHABLE();
    }
}

void Teeko::drop(Hole* hole) {
    if (hole->isEmpty()) {
        hole->setPlayer(m_player);
        ++m_dropCount;

        if (m_dropCount == 8)
            this->setPhase(Teeko::MovePhase);

        emit turnEnded();
    }
}

void Teeko::move(Hole* hole) {
    switch (hole->state()) {
        case Hole::Used:
            if (hole->player() == m_player) {
                this->clearPreviousPlay();

                hole->setState(Hole::Selected);
                foreach (Hole::Direction dir, Hole::directions) {
                    Hole* n = this->neighboor(hole, dir);
                    if (n != nullptr && n->isEmpty())
                        n->setState(Hole::Playable);
                }

                m_selected = hole;
            }

            break;
        case Hole::Playable:
            Q_ASSERT(m_selected != nullptr);
            m_selected->setPlayer(nullptr);
            hole->setPlayer(m_player);
            this->clearPreviousPlay();

            emit turnEnded();
            break;
        default:
            // Nothing to do.
            break;
    }
}

void Teeko::clearPreviousPlay() {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            Hole* hole = m_board[row][col];
            switch (hole->state()) {
                case Hole::Playable:
                    hole->setState(Hole::Empty);
                    break;
                case Hole::Selected:
                    hole->setState(Hole::Used);
                    break;
                default:
                    // Nothing to do.
                    break;
            }
        }
    }

    m_selected = nullptr;
}

Hole* Teeko::neighboor(Hole* hole, Hole::Direction dir) const {
    if (hole == nullptr)
        return nullptr;

    int row, col;
    switch (dir) {
        case Hole::North:
            row = hole->row() - 1;
            col = hole->col();
            break;
        case Hole::NorthEast:
            row = hole->row() - 1;
            col = hole->col() + 1;
            break;
        case Hole::East:
            row = hole->row();
            col = hole->col() + 1;
            break;
        case Hole::SouthEast:
            row = hole->row() + 1;
            col = hole->col() + 1;
            break;
        case Hole::South:
            row = hole->row() + 1;
            col = hole->col();
            break;
        case Hole::SouthWest:
            row = hole->row() + 1;
            col = hole->col() - 1;
            break;
        case Hole::West:
            row = hole->row();
            col = hole->col() - 1;
            break;
        case Hole::NorthWest:
            row = hole->row() - 1;
            col = hole->col() - 1;
            break;
        default:
            Q_UNREACHABLE();
    }

    return ((row >= 0 && row < 5 && col >= 0 && col < 5) ?
                m_board[row][col] : nullptr);
}

bool Teeko::checkWinner() const {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            Hole* hole = m_board[row][col];
            if (hole->isUsed() && hole->player() == m_player) {
                if (this->checkSequence(hole, Hole::North) ||
                        this->checkSequence(hole, Hole::East) ||
                        this->checkSequence(hole, Hole::SouthEast) ||
                        this->checkSequence(hole, Hole::South) ||
                        this->checkSequence(hole, Hole::SouthWest) ||
                        this->checkSequence(hole, Hole::West) ||
                        this->checkSquare(hole))
                    return true;
            }
        }
    }

    return false;
}

bool Teeko::checkSequence(Hole* hole, Hole::Direction dir) const {
    Q_ASSERT(hole != nullptr);
    Q_ASSERT(hole->isUsed());
    Q_ASSERT(hole->player() == m_player);

    for (int i = 0; i < 3; i++) {
        Hole* next = this->neighboor(hole, dir);
        if (next == nullptr || next->state() != Hole::Used ||
            next->player() != m_player)
            return false;

        hole = next;
    }

    return true;
}

bool Teeko::checkSquare(Hole* hole) const {
    Q_ASSERT(hole != nullptr);
    Q_ASSERT(hole->isUsed());
    Q_ASSERT(hole->player() == m_player);

    QList<Hole::Direction> sequence;
    sequence << Hole::East << Hole::SouthEast << Hole::South;
    foreach (Hole::Direction dir, sequence) {
        Hole* next = this->neighboor(hole, dir);
        if (next == nullptr || next->state() != Hole::Used ||
            next->player() != m_player)
            return false;
    }

    return true;
}

void Teeko::switchPlayer() {
    if (this->checkWinner())
        emit gameOver();
    else {
        // Switch the player.
        m_player = m_player->other();

        // Finally, update the status bar.
        this->updateStatusBar();
    }
}

void Teeko::reset() {
    // Reset board.
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            Hole* hole = m_board[row][col];
            hole->reset();
        }
    }

    // Reset the player.
    m_player = Player::player(Player::Red);

    // Reset to drop phase.
    m_phase = Teeko::DropPhase;
    m_dropCount = 0;
    m_selected = nullptr;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Teeko::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Teeko\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Teeko::showGameOver() {
    QMessageBox::information(this, tr("Vencedor"),
        tr("Parabéns, o %1 venceu.")
            .arg(m_player->name()));
}


void Teeko::updateStatusBar() {
    QString phase(m_phase == Teeko::DropPhase ? tr("colocar") : tr("mover"));

    ui->statusbar->showMessage(tr("Fase de %1: vez do %2")
        .arg(phase)
        .arg(m_player->name()));
}

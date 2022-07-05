#include "Alquerque.h"
#include "ui_Alquerque.h"

#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

Alquerque::Player state2player(Hole::State state) {
    switch (state) {
        case Hole::RedState:
            return Alquerque::RedPlayer;
        case Hole::BlueState:
            return Alquerque::BluePlayer;
        default:
            Q_UNREACHABLE();
    }
}

Alquerque::Player otherPlayer(Alquerque::Player player) {
    return (player == Alquerque::RedPlayer ?
                    Alquerque::BluePlayer : Alquerque::RedPlayer);
}

Hole::State player2state(Alquerque::Player player) {
    return player == Alquerque::RedPlayer ? Hole::RedState : Hole::BlueState;
}

Alquerque::Alquerque(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Alquerque),
      m_player(Alquerque::RedPlayer),
      m_mode(MovingMode),
      m_selected(nullptr),
      m_sequence(false) {

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
    QObject::connect(this, SIGNAL(gameOver(Alquerque::Player)), this, SLOT(showGameOver(Alquerque::Player)));
    QObject::connect(this, SIGNAL(gameOver(Alquerque::Player)), this, SLOT(reset()));

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Alquerque::~Alquerque() {
    delete ui;
}

Hole* Alquerque::neighboor(Hole* hole, Hole::Direction dir) {
    if (hole == nullptr)
        return nullptr;

    int row = -1, col = -1;
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

Hole* Alquerque::move(Hole* src, Hole::Direction dir, bool fake) {
    Hole::State playerState = player2state(m_player);
    if (src == nullptr || src->state() != playerState)
        return nullptr;

    Hole* dst = this->neighboor(src, dir);
    if (dst == nullptr || dst->state() != Hole::EmptyState)
        return nullptr;

    // If it is a real move, make it.
    if (!fake) {
        Q_ASSERT(src->isMarked() && dst->isMarked());

        src->setState(Hole::EmptyState);
        src->setMarked(false);

        dst->setState(playerState);
    }

    return dst;
}

QList<Hole::Direction> Alquerque::moveables(Hole* src) {
    QList<Hole::Direction> moveables;

    if (src != nullptr &&
            src->state() == player2state(m_player)) {
        foreach (Hole::Direction dir, src->moves()) {
            Hole* dst = this->move(src, dir, true);
            if (dst != nullptr)
                moveables << dir;
        }
    }

    return moveables;
}

Hole* Alquerque::eat(Hole* src, Hole::Direction dir, bool fake) {
    Hole::State playerState = player2state(m_player);
    if (src == nullptr || src->state() != playerState)
        return nullptr;

    Hole* step = this->neighboor(src, dir);
    if (step == nullptr || step->state() != player2state(otherPlayer(m_player)))
        return nullptr;

    Hole* dst = this->neighboor(step, dir);
    if (dst == nullptr || dst->state() != Hole::EmptyState)
        return nullptr;

    // If it is a real move, make it.
    if (!fake) {
        Q_ASSERT(src->isMarked() && dst->isMarked());

        src->setState(Hole::EmptyState);
        src->setMarked(false);

        step->setState(Hole::EmptyState);

        dst->setState(playerState);
    }

    return dst;
}

QList<Hole::Direction> Alquerque::eatables(Hole* src) {
    QList<Hole::Direction> eatables;

    if (src != nullptr &&
            src->state() == player2state(m_player)) {
        foreach (Hole::Direction dir, src->moves()) {
            Hole* dst = this->eat(src, dir, true);
            if (dst != nullptr)
                eatables << dir;
        }
    }

    return eatables;
}

Hole::Direction Alquerque::findDirection(Hole* hole) {
    Q_ASSERT(m_selected != nullptr);
    Q_ASSERT(hole != nullptr && hole->state() == Hole::EmptyState);

    // Find the correct direction of the selected hole.
    foreach (Hole::Direction dir, m_movements[m_selected]) {
        Hole* dst;
        switch (m_mode) {
            case Alquerque::EatingMode:
                // Check if we can eat it.
                dst = this->eat(m_selected, dir, true);

                // If it is the right direction, eat it.
                if (dst == hole)
                    return dir;

                break;
            case Alquerque::MovingMode:
                // Check if we can move it.
                dst = this->move(m_selected, dir, true);

                // If it is the right direction, move it.
                if (dst == hole)
                    return dir;

                break;
            default:
                Q_UNREACHABLE();
                break;
        }
    }

    Q_UNREACHABLE();
    return Hole::North;
}

void Alquerque::select(Hole* hole) {
    Q_ASSERT(m_selected == nullptr);
    Q_ASSERT(hole != nullptr && hole->state() == player2state(m_player));

    // Now select the new hole.
    m_selected = hole;
    m_selected->setMarked(true);
    foreach (Hole::Direction dir, m_movements[m_selected]) {
        Hole* dst = (m_mode == Alquerque::EatingMode ?
                        this->eat(m_selected, dir, true) : this->move(m_selected, dir, true));
        Q_ASSERT(dst != nullptr);
        dst->setMarked(true);
    }
}

void Alquerque::deselect() {
    Q_ASSERT(m_selected != nullptr);

    m_selected->setMarked(false);
    foreach (Hole::Direction dir, m_movements[m_selected]) {
        Hole* dst = (m_mode == Alquerque::EatingMode ?
                        this->eat(m_selected, dir, true) : this->move(m_selected, dir, true));
        Q_ASSERT(dst != nullptr);
        dst->setMarked(false);
    }

    m_selected = nullptr;
}

bool Alquerque::nextSequence(Hole* hole) {
    Q_ASSERT(hole != nullptr && hole->state() == player2state(m_player));

    QList<Hole::Direction> eatables = this->eatables(hole);
    if (eatables.isEmpty())
        return false;

    // Make the preplay again.
    bool valid = this->preplay();
    Q_ASSERT(valid);

    // Select this hole again.
    Q_ASSERT(m_mode == Alquerque::EatingMode);
    Q_ASSERT(m_movements.contains(hole));
    this->select(hole);

    // And mark it as a sequence.
    m_sequence = true;

    return true;
}

bool Alquerque::preplay() {
    // Count players.
    int count = 0;

    // Get player state and counts.
    Hole::State playerState = player2state(m_player);

    // Reset board and check movements.
    QMap<Hole*, QSet<Hole::Direction>> eatables;
    QMap<Hole*, QSet<Hole::Direction>> moveables;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            Hole* hole = m_board[row][col];
            hole->setMarked(false);
            if (hole->state() == playerState) {
                hole->setEnabled(false);
                count++;
            } else {
                hole->setEnabled(true);
            }

            QList<Hole::Direction> directions;
            if ((directions = this->eatables(hole)).count() > 0) {
                foreach (Hole::Direction dir, directions)
                    eatables[hole] << dir;
            } else if ((directions = this->moveables(hole)).count() > 0) {
                foreach (Hole::Direction dir, directions)
                    moveables[hole] << dir;
            }
        }
    }

    // Check if there are no more plays.
    if (count == 0)
        return false;

    // Update movements and mode.
    if (!eatables.empty()) {
        m_movements = eatables;
        m_mode = Alquerque::EatingMode;
    } else {
        m_movements = moveables;
        m_mode = Alquerque::MovingMode;
    }

    // Check if there are no more movements left.
    if (m_movements.empty())
        return false;

    // Reset selected and sequence.
    m_selected = nullptr;
    m_sequence = false;

    // Enable holes that can be selected.
    foreach (Hole* hole, m_movements.keys())
        hole->setEnabled(true);

    return true;
}

void Alquerque::play(int id) {
    Hole* hole = m_board[id / 5][id % 5];
    Hole::State playerState = player2state(m_player);

    // If there is a selection in place.
    if (m_selected != nullptr) {
        // Check if it is a sequence, in this case we are obligated to proceed.
        if (m_sequence) {
            if (hole->isMarked() && hole->state() == Hole::EmptyState) {
                Hole::Direction dir = this->findDirection(hole);

                Q_ASSERT(m_mode == EatingMode);
                Hole* dst = this->eat(m_selected, dir);
                Q_ASSERT(dst != nullptr);

                // Check if this has a eating sequence.
                if (!this->nextSequence(dst))
                    emit turnEnded();
            }
        } else {
            // Check if this hole is marked.
            if (hole->isMarked()) {
                // Ignore if it is not the empty state.
                if (hole->state() != Hole::EmptyState)
                    return;

                Hole::Direction dir = this->findDirection(hole);
                Hole* dst;
                switch (m_mode) {
                    case Alquerque::EatingMode:
                        // Eat it.
                        dst = this->eat(m_selected, dir);
                        Q_ASSERT(dst != nullptr);

                        // Check if this has a eating sequence.
                        if (!this->nextSequence(dst))
                            emit turnEnded();

                        break;
                    case Alquerque::MovingMode:
                        // Move it.
                        dst = this->move(m_selected, dir);
                        Q_ASSERT(dst != nullptr);

                        // And finish the turn.
                        emit turnEnded();

                        break;
                    default:
                        Q_UNREACHABLE();
                        break;
                }
            // Are we choosing another hole to select?
            } else if (hole->state() == playerState) {
                // First, detach the previously selected hole.
                this->deselect();

                // Then, attach the new hole.
                this->select(hole);
            }
        }
    } else {
        // Ignore if it is not a valid movement.
        if (!m_movements.contains(hole))
            return;

        // Select this hole.
        this->select(hole);
    }
}

void Alquerque::switchPlayer() {
    // Switch the player.
    m_player = otherPlayer(m_player);

    // Prepare the board with playable moves.
    if (this->preplay()) {
        // Update the status bar.
        this->updateStatusBar();
    } else {
        // If there are no plays, then it is a game over.
        emit gameOver(otherPlayer(m_player));
    }
}

void Alquerque::reset() {
    // Reset board.
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            Hole* hole = m_board[row][col];
            hole->reset();

            if (row < 2 || (row == 2 && col < 2))
                hole->setState(Hole::RedState);
            else if (row > 2 || (row == 2 && col > 2))
                hole->setState(Hole::BlueState);
        }
    }

    // Reset the player.
    m_player = Alquerque::RedPlayer;

    // Prepare the board with playable moves.
    this->preplay();

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Alquerque::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Alquerque\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Alquerque::showGameOver(Alquerque::Player player) {
    switch (player) {
        case Alquerque::RedPlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o Jogador Vermelho venceu."));
            break;
        case Alquerque::BluePlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o Jogador Azul venceu."));
            break;
        default:
            Q_UNREACHABLE();
            break;
    }
}

void Alquerque::updateStatusBar() {
    QString player(m_player == Alquerque::RedPlayer ? "Vermelho" : "Azul");
    ui->statusbar->showMessage(tr("Vez do Jogador %2").arg(player));
}

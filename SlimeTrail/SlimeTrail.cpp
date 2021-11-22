#include "SlimeTrail.h"
#include "ui_SlimeTrail.h"

#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

SlimeTrail::Player otherPlayer(SlimeTrail::Player player) {
    return (player == SlimeTrail::RedPlayer ?
                    SlimeTrail::BluePlayer : SlimeTrail::RedPlayer);
}

SlimeTrail::SlimeTrail(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SlimeTrail),
      m_player(SlimeTrail::RedPlayer),
      m_current(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString holeName = QString("hole%1%2").arg(row).arg(col);
            Hole* hole = this->findChild<Hole*>(holeName);
            Q_ASSERT(hole != nullptr);
            Q_ASSERT(hole->row() == row && hole->col() == col);

            m_board[row][col] = hole;

            int id = row * 8 + col;
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

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

SlimeTrail::~SlimeTrail() {
    delete ui;
}

Hole* SlimeTrail::neighboor(Hole* hole, Hole::Direction dir) {
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

    return ((row >= 0 && row < 8 && col >= 0 && col < 8) ?
                m_board[row][col] : nullptr);
}

int SlimeTrail::markNeighboors(Hole* hole, bool mark) {
    Q_ASSERT(hole != nullptr);

    int count = 0;
    foreach (Hole::Direction dir, Hole::directions()) {
        Hole* dst = this->neighboor(hole, dir);
        if (dst != nullptr && dst->state() == Hole::EmptyState) {
            dst->setMarked(mark);

            // Increment counter.
            ++count;
        }
    }

    return count;
}

void SlimeTrail::play(int id) {
    Hole* hole = m_board[id / 8][id % 8];

    // Ignore if hole is not marked.
    if (!hole->isMarked())
        return;

    Q_ASSERT(hole->state() == Hole::EmptyState);
    Q_ASSERT(m_current != nullptr && m_current->state() == Hole::WhiteState);

    // Set current hole as black and rollback previous markings.
    m_current->setState(Hole::BlackState);
    this->markNeighboors(m_current, false);

    // Update current as the new hole, make it white and mark neighboors.
    m_current = hole;
    m_current->setState(Hole::WhiteState);

    int count = this->markNeighboors(m_current, true);
    if (!count ||
        (m_current->row() == 7 && m_current->col() == 0) ||
        (m_current->row() == 0 && m_current->col() == 7)) {
        emit gameOver();
    } else {
        emit turnEnded();
    }
}

void SlimeTrail::switchPlayer() {
    // Switch the player.
    m_player = otherPlayer(m_player);

    // Finally, update the status bar.
    this->updateStatusBar();
}

void SlimeTrail::reset() {
    // Reset board.
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Hole* hole = m_board[row][col];
            hole->reset();
        }
    }

    // Mark the starting position.
    m_current = m_board[3][4];
    m_current->setState(Hole::WhiteState);
    this->markNeighboors(m_current);

    // Reset the player.
    m_player = SlimeTrail::RedPlayer;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void SlimeTrail::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Rastros\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void SlimeTrail::showGameOver() {
    Q_ASSERT(m_current != nullptr);

    if (m_current->row() == 7 && m_current->col() == 0) {
        QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o Jogador Vermelho venceu."));
    } else if (m_current->row() == 0 && m_current->col() == 7) {
        QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o Jogador Azul venceu."));
    } else {
        QMessageBox::information(this, tr("Empate"), tr("O jogo empatou!"));
    }
}

void SlimeTrail::updateStatusBar() {
    QString player(m_player == SlimeTrail::RedPlayer ? "Vermelho" : "Azul");
    ui->statusbar->showMessage(tr("Vez do Jogador %2").arg(player));
}

#include "Catch.h"
#include "ui_Catch.h"
#include "Player.h"

#include <QQueue>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

Catch::Catch(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Catch),
      m_player(Player::player(Player::Red)) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString cellName = QString("cell%1%2").arg(row).arg(col);
            Cell* cell = this->findChild<Cell*>(cellName);
            Q_ASSERT(cell != nullptr);
            Q_ASSERT(cell->row() == row && cell->col() == col);

            m_board[row][col] = cell;

            int id = row * 8 + col;
            map->setMapping(cell, id);
            QObject::connect(cell, SIGNAL(clicked(bool)), map, SLOT(map()));
            QObject::connect(cell, SIGNAL(mouseOver(bool)), this, SLOT(updateSelectables(bool)));
            QObject::connect(cell, SIGNAL(playerChanged(Player*)), this, SLOT(updateCount(Player*)));
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player and check if the game is over.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(checkGameOver()));

    // Notify when the game is over and reset.
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(showGameOver()));
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(reset()));

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Catch::~Catch() {
    delete ui;
}

Cell* Catch::neighboor(Cell* cell, Cell::Direction dir) {
    Q_ASSERT(cell != nullptr);

    int row = cell->row();
    int col = cell->col();
    switch (dir) {
        case Cell::North:
            --row;
            break;
        case Cell::East:
            ++col;
            break;
        case Cell::South:
            ++row;
            break;
        case Cell::West:
            --col;
            break;
        default:
            Q_UNREACHABLE();
    }

    return row >= 0 && row < 8 && col >= 0 && col < 8 ?
                m_board[row][col] : nullptr;
}

QList<QSet<Cell*> > Catch::findClusters() {
    QSet<Cell*> all;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Cell* cell = m_board[row][col];
            if (cell->isEmpty())
                all.insert(cell);
        }
    }

    QList<QSet<Cell*> > clusters;
    while (!all.isEmpty()) {
        QSet<Cell*> cluster;

        Cell* cell = *(all.begin());
        Q_ASSERT(cell != nullptr);

        QQueue<Cell*> worklist;
        worklist << cell;

        while (!worklist.isEmpty()) {
            cell = worklist.dequeue();
            cluster.insert(cell);
            all.remove(cell);

            foreach (Cell::Direction dir, Cell::directions) {
                Cell* neighboor = this->neighboor(cell, dir);
                if (neighboor != nullptr &&
                        neighboor->isEmpty() &&
                        all.contains(neighboor) &&
                        !worklist.contains(neighboor) &&
                        !cluster.contains(neighboor))
                    worklist.enqueue(neighboor);
            }
        }

        clusters << cluster;
    }

    return clusters;
}

void Catch::play(int id) {
    Cell* cell = m_board[id / 8][id % 8];
    if (cell == nullptr || !cell->isSelectable())
        return;

    Cell* neighboor = this->neighboor(cell,
            m_player->orientation() == Player::Vertical ? Cell::South : Cell::East);
    if (neighboor == nullptr || !neighboor->isSelectable())
        return;

    cell->setState(Cell::Blocked);
    neighboor->setState(Cell::Blocked);

    QList<QSet<Cell*> > clusters = this->findClusters();
    foreach (const QSet<Cell*>& cluster, clusters) {
        if (cluster.count() <= 3) {
            foreach (Cell* c, cluster) {
                c->setPlayer(m_player);
            }
        }
    }

    emit turnEnded();
}

void Catch::switchPlayer() {
    // Switch the player.
    m_player = m_player->other();

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Catch::reset() {
    // Reset board.
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Cell* cell = m_board[row][col];
            cell->reset();
        }
    }

    // Reset the players.
    Player* red = Player::player(Player::Red);
    red->reset();

    Player* blue = Player::player(Player::Blue);
    blue->reset();

    m_player = red;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Catch::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Catch\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Catch::showGameOver() {
    Player* players[] = { m_player, m_player->other() };
    int counts[] = { players[0]->count(), players[1]->count() };

    if (counts[0] == counts[1])
       QMessageBox::information(this, tr("Empate"),
            tr("O jogo empatou em %1 a %2.")
                .arg(counts[0])
                .arg(counts[1]));
    else {
        int idx = counts[0] > counts[1] ? 0 : 1;
        QMessageBox::information(this, tr("Vencedor"),
            tr("Parabéns, o %1 venceu por %2 a %3.")
                .arg(players[idx]->name())
                .arg(counts[idx])
                .arg(counts[(idx + 1) % 2]));
    }
}

void Catch::updateSelectables(bool over) {
    Cell* cell = qobject_cast<Cell*>(QObject::sender());
    Q_ASSERT(cell != nullptr);

    Cell* neighboor = this->neighboor(cell,
        m_player->orientation() == Player::Vertical ? Cell::South : Cell::East);
    if (neighboor == nullptr)
        return; // ignore

    if (over) {
        if (cell->isEmpty() && neighboor->isEmpty()) {
            cell->setState(Cell::Selectable);
            neighboor->setState(Cell::Selectable);
        }
    } else {
        if (cell->isSelectable() && neighboor->isSelectable()) {
            cell->setState(Cell::Empty);
            neighboor->setState(Cell::Empty);
        }
    }
}

void Catch::updateCount(Player* player) {
    player->incrementCount();
}

void Catch::updateStatusBar() {
    ui->statusbar->showMessage(tr("Vez do %1 (%2 a %3)")
        .arg(m_player->name()).arg(m_player->count()).arg(m_player->other()->count()));
}

void Catch::checkGameOver() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Cell* cell = m_board[row][col];
            if (cell->isEmpty()) {
                Cell* neighboor = this->neighboor(cell,
                        m_player->orientation() == Player::Vertical ? Cell::South : Cell::East);
                if (neighboor != nullptr && neighboor->isEmpty())
                    return;
            }
        }
    }

    emit gameOver();
}

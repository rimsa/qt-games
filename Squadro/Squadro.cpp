#include "Squadro.h"
#include "ui_Squadro.h"
#include "Cell.h"
#include "Lane.h"
#include "Player.h"

#include <QDebug>
#include <QMessageBox>
#include <QSignalMapper>

Squadro::Squadro(QWidget *parent)
    : QMainWindow(parent),
        ui(new Ui::Squadro),
        m_player(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString cellName = QString("cell%1%2").arg(row).arg(col);
            Cell* cell = this->findChild<Cell*>(cellName);
            m_board[row][col] = cell;

            if (cell != nullptr) {
                int id = row * 7 + col;
                map->setMapping(cell, id);
                QObject::connect(cell, SIGNAL(clicked(bool)), map, SLOT(map()));
            }
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // Setup the steps for each lane.
    QList<int> steps;
    steps << 1 << 3 << 2 << 3 << 1;

    // Build lanes for the red player.
    Player* red = Player::player(Player::Red);
    QObject::connect(red, SIGNAL(countChanged(int)), this, SLOT(updateStatusBar()));
    for (int row = 1; row <= 5; row++) {
        QList<Cell*> cells;
        for (int col = 0; col < 7; col++)
            cells << m_board[row][col];

        int s = steps.at(row-1);
        Lane* lane = new Lane(red, cells, s, (4 - s), this);
        red->addLane(lane);
    }

    // Build lanes for the blue player.
    Player* blue = Player::player(Player::Blue);
    QObject::connect(blue, SIGNAL(countChanged(int)), this, SLOT(updateStatusBar()));
    for (int col = 1; col <= 5; col++) {
        QList<Cell*> cells;
        for (int row = 6; row >= 0; row--)
            cells << m_board[row][col];

        int s = steps.at(col-1);
        Lane* lane = new Lane(blue, cells, s, (4 - s), this);
        blue->addLane(lane);
    }

    // When the turn ends, switch the player.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));

    // When the game is over, show message and reset.
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(showGameOver()));
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(reset()));

    // Reset.
    this->reset();

    // Adjust window.
    this->adjustSize();
    this->setFixedSize(this->size());
}

Squadro::~Squadro() {
    delete ui;
}

void Squadro::play(int id) {
    int row = id / 7;
    int col = id % 7;
    Cell* cell = m_board[row][col];
    Q_ASSERT(cell != nullptr);

    if (cell->player() != m_player)
        return;

    Lane* lane = m_player->findLane(cell);
    Q_ASSERT(lane->current() == cell);
    if (lane->isFinished())
        return;

    lane->move();

    if (m_player->count() == 4)
        emit gameOver();
    else
        emit turnEnded();
}

void Squadro::switchPlayer() {
    // Switch the player.
    m_player = m_player->other();

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Squadro::reset() {
    // Reset the red player.
    Player* red = Player::player(Player::Red);
    red->reset();

    // Reset the blue player.
    Player* blue = Player::player(Player::Blue);
    blue->reset();

    // Set the starting player.
    m_player = red;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Squadro::showAbout() {
    QMessageBox::information(this, tr("About"),
        tr("Squadro\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Squadro::showGameOver() {
    QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o %1 venceu.").arg(m_player->name()));
}

void Squadro::updateStatusBar() {
    ui->statusbar->showMessage(tr("Vez do %1 (%2 de 5)").arg(m_player->name()).arg(m_player->count()));
}

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
      m_player(Alquerque::RedPlayer) {

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

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Alquerque::~Alquerque() {
    delete ui;
}

void Alquerque::play(int id) {
    Hole* hole = m_board[id / 5][id % 5];
    qDebug() << "clicked on: " << hole->objectName();

    hole->setState(player2state(m_player));
    emit turnEnded();
}

void Alquerque::switchPlayer() {
    // Remove all markings.
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            Hole* hole = m_board[row][col];
            hole->setMarked(false);
        }
    }

    // Switch the player.
    m_player = otherPlayer(m_player);

    // Finally, update the status bar.
    this->updateStatusBar();
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

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Alquerque::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Alquerque\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Alquerque::updateStatusBar() {
    QString player(m_player == Alquerque::RedPlayer ? "Vermelho" : "Azul");
    ui->statusbar->showMessage(tr("Vez do Jogador %2").arg(player));
}

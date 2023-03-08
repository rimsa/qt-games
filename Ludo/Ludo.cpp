#include "Ludo.h"
#include "ui_Ludo.h"
#include "Home.h"
#include "Pawn.h"
#include "Track.h"
#include <QActionGroup>
#include <QMessageBox>

Ludo::Ludo(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Ludo),
          m_current(0), m_count(2), m_action(Ludo::Roll), m_return(false), m_sixes(0),
          m_players{ Player::player(Player::Red), Player::player(Player::Yellow),
                     Player::player(Player::Green), Player::player(Player::Blue)} {

    ui->setupUi(this);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(ui->actionMode2P);
    group->addAction(ui->actionMode3P);
    group->addAction(ui->actionMode4P);
    QObject::connect(group, SIGNAL(triggered(QAction*)), this, SLOT(changeMode(QAction*)));

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(this, SIGNAL(countChanged(int)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QObject::connect(ui->dice, SIGNAL(rolled(int)), this, SLOT(handleRoll(int)));

    for (int idx = 0; idx < 4; idx++) {
        Home* home = m_players[idx]->home();
        QObject::connect(home, SIGNAL(pawnSelected(Pawn*)), this, SLOT(movePawnFromHome(Pawn*)));

        Track* track = m_players[idx]->track();
        QObject::connect(track, SIGNAL(pawnSelected(Pawn*)), this, SLOT(movePawnOnTrack(Pawn*)));
        QObject::connect(track, SIGNAL(pawnReturned(Pawn*)), this, SLOT(returnPawnToHome(Pawn*)));
        QObject::connect(track, SIGNAL(finished(Pawn*)), this, SLOT(movePawnToStatus(Pawn*)));
    }

    QObject::connect(this, SIGNAL(actionChanged(Ludo::Action)), this, SLOT(handleAction(Ludo::Action)));
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));
    QObject::connect(this, SIGNAL(gameOver(Player*)), this, SLOT(showGameOver(Player*)));
    QObject::connect(this, SIGNAL(gameOver(Player*)), this, SLOT(reset()));

    this->reset();

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

Ludo::~Ludo() {
    delete ui;
}

void Ludo::reset() {
    Player* player;
    Home* home;
    Track* track;
    Status* status;

    for (int idx = 0; idx < 4; idx++) {
        player = m_players[idx];
        player->reset();

        status = player->status();
        status->reset();

        track = player->track();
        track->reset();

        home = player->home();
        home->reset();

        bool active = idx < m_count;
        if (active) {
            foreach (Pawn* pawn, player->pawns())
                home->insertPawn(pawn);

            status->show();
        } else {
            status->hide();
        }
    }

    m_current = 0;
    player = this->current();

    m_return = false;
    m_sixes = 0;
    m_action = Ludo::Roll;

    ui->dice->setType(player->type());
    ui->dice->reset();
    ui->dice->setEnabled(true);

    status = player->status();
    status->setTurn(true);
}

void Ludo::changeMode(QAction* action) {
    if (action == ui->actionMode2P) {
        if (m_count != 2) {
            m_count = 2;
            emit countChanged(m_count);
        }
    } else if (action == ui->actionMode3P) {
        if (m_count != 3) {
            m_count = 3;
            emit countChanged(m_count);
        }
   } else if (action == ui->actionMode4P) {
        if (m_count != 4) {
            m_count = 4;
            emit countChanged(m_count);
        }
    } else {
        Q_UNREACHABLE();
    }
}

void Ludo::setAction(Ludo::Action action) {
    if (m_action != action) {
        m_action = action;
        emit actionChanged(action);
    }
}

void Ludo::showAbout() {
    QMessageBox::information(this, tr("Sobre"),
        tr("Ludo\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Ludo::showGameOver(Player* player) {
    QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o %1 venceu.").arg(player->name()));
}

void Ludo::handleRoll(int n) {
    Player* player = this->current();

    if (n == 6)
        ++m_sixes;

    if (m_sixes == 3)
        m_return = false;
    else {
        bool playable = false;

        Home* home = player->home();
        playable |= home->markPlayablePawns(n);

        Track* track = player->track();
        playable |= track->markPlayablePawns(n);

        if (playable) {
            this->setAction(Ludo::Move);
            return;
        }
    }

    QMessageBox::information(this, tr("Jogada"), tr("%1 passou a vez").arg(player->name()));
    emit turnEnded();
}

void Ludo::handleAction(Ludo::Action action) {
    ui->dice->setEnabled(action == Ludo::Roll);
}

void Ludo::switchPlayer() {
    Player* player = this->current();

    Status* status = player->status();
    if (status->finished() == 4) {
        emit gameOver(player);
        return;
    }

    status->setTurn(false);
    foreach (Pawn* pawn, player->pawns())
        pawn->setPlayable(false);

    // Check if the player has another turn.
    if (m_return) {
        m_return = false;
    } else {
        m_current = (m_current + 1) % m_count;
        player = this->current();

        m_sixes = 0;
    }

    foreach (Pawn* pawn, player->pawns())
        emit pawn->raised();

    player->status()->setTurn(true);

    ui->dice->setType(player->type());
    this->setAction(Ludo::Roll);
}

void Ludo::movePawnFromHome(Pawn* pawn) {
    if (this->action() == Ludo::Move && pawn->isPlayable()) {
        Player* player = this->current();
        Q_ASSERT(player == pawn->player());

        Home* home = player->home();
        home->removePawn(pawn);

        Track* track = player->track();
        track->insertPawn(pawn);

        m_return = true;

        emit turnEnded();
    }
}

void Ludo::returnPawnToHome(Pawn* pawn) {
    Player* player = pawn->player();

    Track* track = player->track();
    track->removePawn(pawn);

    Home* home = player->home();
    home->insertPawn(pawn);
}

void Ludo::movePawnOnTrack(Pawn* pawn) {
    if (this->action() == Ludo::Move && pawn->isPlayable()) {
        Player* player = this->current();
        Q_ASSERT(player == pawn->player());

        Track* track = player->track();
        if (track->advance(pawn, ui->dice->last()))
            m_return = true;

        emit turnEnded();
    }
}

void Ludo::movePawnToStatus(Pawn* pawn) {
    if (this->action() == Ludo::Move && pawn->isPlayable()) {
        Player* player = this->current();
        Q_ASSERT(player == pawn->player());

        Track* track = player->track();
        track->removePawn(pawn);

        Status* status = player->status();
        status->insertPawn(pawn);
    }
}

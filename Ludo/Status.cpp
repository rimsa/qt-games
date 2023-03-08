#include "Status.h"
#include "ui_Status.h"
#include "Pawn.h"

Status::Status(QWidget *parent)
        : QWidget(parent), ui(new Ui::Status),
          m_player(Player::player(Player::Empty)),
          m_turn(false) {

    ui->setupUi(this);

    for (int idx = 0; idx < 4; idx++) {
        Tile* tile = this->findChild<Tile*>(QString("finish%1").arg(idx));
        Q_ASSERT(tile != nullptr);

        m_finish[idx] = tile;
    }

    QObject::connect(this, SIGNAL(typeChanged(Player::Type)), this, SLOT(updateStatus()));
    QObject::connect(this, SIGNAL(turnChanged(bool)), this, SLOT(updateStatus()));

    this->reset();
}

Status::~Status() {
    delete ui;
}

void Status::setType(Player::Type type) {
    if (type != this->type()) {
        m_player = Player::player(type);
        m_player->m_status = this;
        emit typeChanged(type);
    }
}

void Status::setTurn(bool turn) {
    if (turn != m_turn) {
        m_turn = turn;
        emit turnChanged(turn);
    }
}

void Status::insertPawn(Pawn* pawn) {
    int count = m_pawns.count();
    Q_ASSERT(count < 4);
    Q_ASSERT(!m_pawns.contains(pawn));
    Q_ASSERT(pawn->player() == this->player());

    m_pawns << pawn;

    pawn->setPlayable(false);
    m_finish[count++]->insertPawn(pawn);

    emit pawnInserted(pawn);
}

void Status::reset() {
    m_turn = false;

    foreach (Pawn* pawn, m_pawns)
        pawn->setOwner(nullptr);

    m_pawns.clear();

    for (int idx = 0; idx < 4; idx++) {
        m_finish[idx]->reset();
        m_finish[idx]->setIcon(QPixmap(":/pawns/empty"));
    }

    this->updateStatus();
}

void Status::updateStatus() {
    ui->turn->setPixmap(m_turn ? QPixmap(":/others/dot") : QPixmap());

    ui->name->setText(QString("%1: ").arg(m_player->name()));
    ui->name->setStyleSheet(QString("color: %1").arg(m_player->color().name()));
}

#include "Dice.h"
#include "Player.h"
#include <QTime>

QString Dice::sscode("#dice {\
  border: 2px solid black;\
  border-radius: 12px;\
}\
#dice::enabled {\
    background-color: %1;\
}");

Dice::Dice(QWidget *parent)
        : QPushButton(parent), m_last(1),
          m_player(Player::player(Player::Empty)) {

    qsrand(QTime::currentTime().msec());

    for (int f = 0; f < 6; f++)
        m_faces[f] = QPixmap(QString(":/dices/face%1").arg(f+1));

    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(roll()));
    QObject::connect(this, SIGNAL(typeChanged(Player::Type)), this, SLOT(updatePlayerColor()));
}

Dice::~Dice() {
}

void Dice::setType(Player::Type type) {
    if (type != this->type()) {
        m_player = Player::player(type);
        emit typeChanged(type);
    }
}

void Dice::reset() {
    this->setIcon(m_faces[0]);
    this->updatePlayerColor();
}

void Dice::roll() {
    int r = (qrand() % 6);
    this->setIcon(m_faces[r]);

    m_last = r+1;
    emit rolled(m_last);
}

void Dice::updatePlayerColor() {
    this->setStyleSheet(sscode.arg(m_player->color().name()));
}

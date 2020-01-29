#include "Home.h"
#include "ui_Home.h"
#include <QDebug>

Home::Home(QWidget *parent)
    : QWidget(parent), ui(new Ui::Home),
      m_player(Player::player(Player::Empty)), m_active(false) {
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(activeChanged(bool)),
                     this, SLOT(reset()));
}

Home::~Home() {
    delete ui;
}

Player::Type Home::type() const {
    return m_player->type();
}

void Home::setType(Player::Type type) {
    if (type != this->type()) {
        m_player = Player::player(type);
        emit typeChanged(type);
    }
}

bool Home::active() const {
    return m_active;
}

void Home::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        emit activeChanged(active);
    }
}

void Home::reset() {
    QPixmap icon = m_active ? m_player->pixmap() : QPixmap();
    ui->home0->setIcon(icon);
    ui->home1->setIcon(icon);
    ui->home2->setIcon(icon);
    ui->home3->setIcon(icon);
}

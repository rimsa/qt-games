#include "Player.h"

Player* Player::m_players[] = { 0, 0 };
QMutex& Player::m_mutex = *(new QMutex()); // ugly hack

Player::Player(Player::Type type)
    : QObject(0), m_type(type),
      m_name(QString("Player %1").arg(type)),
      m_count(0) {
    switch (type) {
        case Player::Red:
            m_pixmap = QPixmap(":/red");
            break;
        case Player::Blue:
            m_pixmap = QPixmap(":/blue");
            break;
        default:
            Q_UNREACHABLE();
            break;
    }
}

Player::~Player() {
}

Player* Player::player(Player::Type type) {
    if (!m_players[type]) {
        QMutexLocker locker(&m_mutex);
        Q_UNUSED(locker);

        if (!m_players[type])
            m_players[type] = new Player(type);
    }

    return m_players[type];
}

Player* Player::other() const {
    switch (m_type) {
        case Player::Red:
            return m_players[Player::Blue];
        case Player::Blue:
            return m_players[Player::Red];
        default:
            Q_UNREACHABLE();
            return 0;
    }
}

void Player::inc() {
    m_count++;
    emit countChanged(m_count);
}

void Player::dec() {
    m_count--;
    emit countChanged(m_count);
}

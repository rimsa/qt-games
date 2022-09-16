#include "Player.h"

Player* Player::m_players[] = { 0, 0 };
QMutex& Player::m_mutex = *(new QMutex()); // ugly hack

Player::Player(Player::Type type)
    : QObject(0), m_type(type), m_count(0) {
    switch (type) {
        case Player::Red:
            m_name = tr("Jogador Vermelho");
            m_pixmap = QPixmap(":/red");
            break;
        case Player::Blue:
            m_name = tr("Jogador Azul");
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
            return Player::player(Player::Blue);
        case Player::Blue:
            return Player::player(Player::Red);
        default:
            Q_UNREACHABLE();
            return 0;
    }
}

void Player::reset() {
    m_count = 0;
}

void Player::incrementCount() {
    m_count++;
    emit countChanged(m_count);
}

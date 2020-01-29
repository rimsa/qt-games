#include "Player.h"

Player* Player::m_players[] = { 0, 0, 0, 0, 0 };
QMutex& Player::m_mutex = *(new QMutex()); // ugly hack

Player::Player(Player::Type type)
    : QObject(0), m_type(type),
      m_name(QString("Player %1").arg(type)) {
    switch (type) {
        case Player::Empty:
            m_color = Qt::transparent;
            m_pixmap = QPixmap(":/pawns/empty");
            break;
        case Player::Red:
            m_color = Qt::red;
            m_pixmap = QPixmap(":/pawns/red");
            break;
        case Player::Green:
            m_color = Qt::green;
            m_pixmap = QPixmap(":/pawns/green");
            break;
        case Player::Blue:
            m_color = Qt::blue;
            m_pixmap = QPixmap(":/pawns/blue");
            break;
        case Player::Yellow:
            m_color = Qt::yellow;
            m_pixmap = QPixmap(":/pawns/yellow");
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

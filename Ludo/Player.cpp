#include "Player.h"
#include "Pawn.h"

Player* Player::m_players[] = { 0, 0, 0, 0, 0 };
QMutex& Player::m_mutex = *(new QMutex()); // ugly hack

Player::Player(Player::Type type)
        : QObject(0), m_type(type),
          m_name(QString("Jogador %1").arg(type)) {

    switch (type) {
        case Player::Empty:
            m_color = Qt::transparent;
            m_pixmap[0] = m_pixmap[1] = QPixmap();
            break;
        case Player::Red:
            m_color = Qt::red;
            m_pixmap[0] = QPixmap(":/pawns/redstd");
            m_pixmap[1] = QPixmap(":/pawns/redplay");
            break;
        case Player::Green:
            m_color = Qt::green;
            m_pixmap[0] = QPixmap(":/pawns/greenstd");
            m_pixmap[1] = QPixmap(":/pawns/greenplay");
            break;
        case Player::Blue:
            m_color = Qt::blue;
            m_pixmap[0] = QPixmap(":/pawns/bluestd");
            m_pixmap[1] = QPixmap(":/pawns/blueplay");
            break;
        case Player::Yellow:
            m_color = Qt::yellow;
            m_pixmap[0] = QPixmap(":/pawns/yellowstd");
            m_pixmap[1] = QPixmap(":/pawns/yellowplay");
            break;
        default:
            Q_UNREACHABLE();
            break;
    }

    if (type != Player::Empty) {
        for (int i = 0; i < 4; i++)
            m_paws << new Pawn(this);
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

Pawn* Player::pawn(int id) const {
    Q_ASSERT(id >= 0 && id < m_paws.count());
    return m_paws.at(id);
}

const QList<Pawn*>& Player::pawns() const {
    return m_paws;
}

void Player::reset() {
    foreach (Pawn* pawn, m_paws) {
        pawn->setPlayable(false);
        pawn->setOwner(nullptr);
    }
}

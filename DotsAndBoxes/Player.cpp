#include "Player.h"

int Player::next_id = 0;

Player::Player(const QString& name, const QColor& color, QObject* parent)
    : QObject(parent), m_id(next_id++), m_name(name), m_color(color), m_count(0) {
}

Player::~Player() {
}

void Player::reset() {
    m_count = 0;
}

void Player::increaseCount() {
    m_count++;

    emit countChanged(m_count);
    if (m_count > 4)
        emit gameOver();
}

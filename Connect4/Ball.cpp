#include <Ball.h>

Ball::Ball(QWidget* parent)
    : QLabel(parent), m_color(Ball::BLANK) {
}

Ball::~Ball() {
}

QPixmap Ball::colorToPixmap(Ball::Color color) {
    switch (color) {
        case Ball::RED:
            return QPixmap(":/images/red");
        case Ball::BLUE:
            return QPixmap(":/images/blue");
        default:
            return QPixmap();
    }
}

QString Ball::colorToString(Ball::Color color) {
    switch (color) {
        case Ball::RED:
            return trUtf8("Vermelho");
        case Ball::BLUE:
            return trUtf8("Azul");
        default:
            return trUtf8("Branco");
    }
}

void Ball::setColor(Ball::Color color) {
    if (m_color != color) {
        // Change the color.
        m_color = color;

        // Update the pixmap with the correct color.
        this->setPixmap(Ball::colorToPixmap(color));

        // Emit a signal notifying a color change.
        emit colorChanged(color);
    }
}

void Ball::reset() {
    m_color = Ball::BLANK;
    this->setPixmap(Ball::colorToPixmap(m_color));
}

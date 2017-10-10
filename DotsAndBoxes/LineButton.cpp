#include <QStyle>
#include <Player.h>
#include <LineButton.h>

#include <QDebug>

LineButton::LineButton(QWidget *parent) : QPushButton(parent), m_mode(Qt::Horizontal), m_player(0) {
    this->updateStyleSheet();

    QObject::connect(this, SIGNAL(modeChanged(Qt::Orientation)), this, SLOT(updateStyleSheet()));
    QObject::connect(this, SIGNAL(playerSet(Player*)), this, SLOT(updateStyleSheet()));
}

LineButton::~LineButton() {
}

void LineButton::setMode(Qt::Orientation mode) {
    if (m_mode != mode) {
        m_mode = mode;
        emit modeChanged(m_mode);
    }
}

void LineButton::setPlayer(Player* player) {
    Q_ASSERT(player);

    if (!m_player) {
        m_player = player;
        this->setEnabled(false);

        emit playerSet(player);
    }
}

void LineButton::reset() {
    m_player = 0;
    this->setEnabled(true);
}

void LineButton::updateStyleSheet() {
    QString rgba = (m_player ? m_player->color() : Qt::transparent).name(QColor::HexArgb);

    this->setStyleSheet(QString("  \
QPushButton {                      \
    border-radius: 0px;            \
    border: 0px solid transparent; \
}                                  \
QPushButton:enabled:hover {        \
    border-image: url(\":/dotts\"); \
}                                  \
QPushButton[mode=\"1\"] {          \
    border-top-width: 15px;        \
}                                  \
QPushButton[mode=\"1\"]:!enabled { \
    border-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,        \
                    stop:0 white, stop: 0.2 white, stop: 0.5 %1, \
                    stop: 0.8 white, stop:1 white);              \
}                                                                \
QPushButton[mode=\"2\"] {          \
    border-left-width: 15px;       \
}                                  \
QPushButton[mode=\"2\"]:!enabled { \
    border-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,        \
                stop:0 white, stop: 0.2 white, stop: 0.5 %1, \
                stop: 0.8 white, stop:1 white);              \
}                                  \
    ").arg(rgba));
}

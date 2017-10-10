#include <Player.h>
#include <BoxLabel.h>
#include <LineButton.h>

BoxLabel::BoxLabel(QWidget *parent) : QLabel(parent), m_count(0) {
}

BoxLabel::~BoxLabel() {
}

void BoxLabel::addLine(LineButton* button) {
    QObject::connect(button, SIGNAL(playerSet(Player*)), this, SLOT(updateCount(Player*)));
}

void BoxLabel::reset() {
    m_count = 0;
    this->setText("");
}

void BoxLabel::updateCount(Player* player) {
    if (++m_count == 4) {
        this->setText(player->name());

        QPalette pal = this->palette();
        pal.setColor(QPalette::WindowText, player->color());
        this->setPalette(pal);

        player->increaseCount();

        emit boxMarked();
    }
}

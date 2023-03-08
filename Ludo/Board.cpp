#include "Board.h"
#include "ui_Board.h"
#include "Track.h"
#include <QMetaEnum>

Board::Board(QWidget *parent)
        : QWidget(parent), ui(new Ui::Board) {
    ui->setupUi(this);

    for (int i = 0; i < 52; i++) {
        QString tileName = QString("tile%1").arg(i, 2, 10, QChar('0'));
        Tile* tile = this->findChild<Tile*>(tileName);
        Q_ASSERT(tile != nullptr);

        m_tiles.regular[i] = tile;
    }

    for (int i = 0; i < 20; i++) {
        QString tileName = QString("center%1").arg(i, 2, 10, QChar('0'));
        Tile* tile = this->findChild<Tile*>(tileName);
        Q_ASSERT(tile != nullptr);

        m_tiles.special[i] = tile;
    }

    QMetaEnum e = QMetaEnum::fromType<Player::Type>();
    for (int k = 1; k < e.keyCount(); k++) {
        Player::Type type = (Player::Type) e.value(k);
        Player* player = Player::player(type);
        m_tracks[k-1] = new Track(player, this);
    }

    for (int i = 0; i < 51; i++) {
        m_tracks[0]->addTile(m_tiles.regular[(i+3) % 52]);
        m_tracks[1]->addTile(m_tiles.regular[(i+29) % 52]);
        m_tracks[2]->addTile(m_tiles.regular[(i+16) % 52]);
        m_tracks[3]->addTile(m_tiles.regular[(i+42) % 52]);
    }

    for (int i = 0; i < 5; i++) {
        m_tracks[0]->addTile(m_tiles.special[i]);
        m_tracks[1]->addTile(m_tiles.special[i+10]);
        m_tracks[2]->addTile(m_tiles.special[i+5]);
        m_tracks[3]->addTile(m_tiles.special[i+15]);
    }
}

Board::~Board() {
    delete ui;
}

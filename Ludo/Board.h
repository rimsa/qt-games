#ifndef BOARD_H
#define BOARD_H

#include <QWidget>

namespace Ui {
    class Board;
}

class Tile;
class Track;

class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    virtual ~Board();

private:
    Ui::Board *ui;
    struct {
        Tile* regular[52];
        Tile* special[20];
    } m_tiles;
    Track* m_tracks[4];

    friend class Ludo;

};

#endif // BOARD_H

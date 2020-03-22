#ifndef PUZZLE15_H
#define PUZZLE15_H

#include <QMap>
#include <QPushButton>
#include <QMainWindow>

namespace Ui {
    class Puzzle15;
}

class Puzzle15 : public QMainWindow {
    Q_OBJECT

public:
    explicit Puzzle15(QWidget *parent = 0);
    virtual ~Puzzle15();

private:
    Ui::Puzzle15 *ui;
    int m_moves;
    QMap<int, QPushButton*> m_tiles;

    QPushButton* top(int tileNumber) const;
    QPushButton* left(int tileNumber) const;
    QPushButton* bottom(int tileNumber) const;
    QPushButton* right(int tileNumber) const;

    QPushButton* tile(int tileNumber) const;

    bool canMoveTo(QPushButton* button) const;
    void moveTile(QPushButton* from, QPushButton* to);
    void checkBoard();

signals:
    void gameOver();

private slots:
    void handleTileChange(int tileNumber);
    void updateMovements(int moves);
    void shuffleTiles();
    void showAbout();
    void showGameOver();

};

#endif // PUZZLE15_H

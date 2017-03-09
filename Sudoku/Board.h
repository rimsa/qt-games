#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "DefaultBoards.h"

namespace Ui {
    class Board;
}

class Cell;

class Board : public QWidget {
    Q_OBJECT
    Q_PROPERTY(DefaultBoards::Mode mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    explicit Board(QWidget *parent = 0);
    virtual ~Board();

    DefaultBoards::Mode mode() const { return m_mode; }

signals:
    void modeChanged(DefaultBoards::Mode mode);
    void completed(double stats);
    void gameOver();

public slots:
    void renew();
    void clear();

    void setMode(DefaultBoards::Mode mode);

private:
    DefaultBoards::Mode m_mode;
    Cell* m_cells[9][9];
    QList<Cell*> m_all;
    Ui::Board *ui;

    bool checkLine(Cell* cell) const;
    bool checkColumn(Cell* cell) const;
    bool checkBox(Cell* cell) const;

    bool checkCell(Cell* orig, Cell* other) const;

private slots:
    void updateValue();
    void checkBoard();
    void showCongratulations();

};

#endif // BOARD_H

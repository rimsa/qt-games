#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class Player;

class Cell : public QPushButton {
    Q_OBJECT

public:
    explicit Cell(QWidget *parent = 0);
    virtual ~Cell();

    int row() const { return m_row; }
    void setRow(int row) { m_row = row; }

    int col() const { return m_col; }
    void setCol(int col) { m_col = col; }

    Player* owner() const { return m_owner; }
    bool hasOwner() const { return m_owner != 0; }

signals:

public slots:
    void reset();
    void setOwner(Player* player);
    void flip();

private:
    int m_row, m_col;
    Player* m_owner;

};

#endif // CELL_H

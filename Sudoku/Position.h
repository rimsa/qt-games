#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    explicit Position();
    explicit Position(int row, int col);
    virtual ~Position();

    int row() const { return m_row; }
    int col() const { return m_col; }

private:
    int m_row;
    int m_col;

};

#endif // POSITION_H

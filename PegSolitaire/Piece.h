#ifndef PIECE_H
#define PIECE_H

#include <QPushButton>

class Piece : public QPushButton {
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(Piece::State state READ state WRITE setState NOTIFY stateChanged)

    friend class Board;

public:
    enum State {
        Empty,
        Filled,
        Selected,
        Jumpable,
    };

    explicit Piece(QWidget* parent = 0);
    virtual ~Piece();

    int row() const { return m_row; }
    int col() const { return m_col; }

    Piece::State state() const { return m_state; }
    void setState(Piece::State state);

signals:
    void stateChanged(Piece::State oldState, Piece::State newState);

private:
    int m_row;
    int m_col;
    Piece::State m_state;

    void setRow(int row);
    void setCol(int col);

    static QPixmap stateToPixmap(Piece::State state);

private slots:
    void updatePiece();

};

#endif // PIECE_H

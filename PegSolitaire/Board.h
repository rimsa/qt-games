#ifndef BOARD_H
#define BOARD_H

#include <QList>
#include <QObject>

#include <Piece.h>

class Board : public QObject {
    Q_OBJECT
    Q_ENUMS(Direction)

public:
    struct Play {
        Piece* from;
        Piece* over;
        Piece* to;
    };

    enum Direction {
        Up,
        Right,
        Down,
        Left
    };

    explicit Board(QObject *parent = 0);
    virtual ~Board();

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    int count() const { return m_filled + m_selected; }

    void addPiece(int r, int c, Piece* piece);
    Piece* piece(int r, int c) const;

    bool isPlayable(const Board::Play play) const;
    QList<Board::Play> plays(Piece* piece) const;

signals:
    void countChanged(int count);
    void gameOver();

public slots:
    void play(const Board::Play play);

private:
    int m_rows, m_cols;
    int m_filled, m_selected;
    Piece*** m_board;

private slots:
    void updateCount(Piece::State olds, Piece::State news);
    void checkGameOver();

};

#endif // BOARD_H

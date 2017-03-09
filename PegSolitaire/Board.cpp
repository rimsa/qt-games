#include "Board.h"

Board::Board(QObject *parent)
    : QObject(parent),
    m_rows(7), m_cols(7),
    m_filled(0), m_selected(0) {

    m_board = new Piece** [m_rows];
    for (int r = 0; r < 7; r++) {
        m_board[r] = new Piece* [m_cols];
        for (int c = 0; c < 7; c++)
            m_board[r][c] = 0;
    }
}

Board::~Board() {
}

void Board::addPiece(int r, int c, Piece* piece) {
    if ((r >= 0 && r < this->rows()) &&
        (c >= 0 && c < this->cols())) {
        piece->setRow(r);
        piece->setCol(c);

        m_board[r][c] = piece;

        QObject::connect(piece, SIGNAL(stateChanged(Piece::State, Piece::State)),
                         this, SLOT(updateCount(Piece::State, Piece::State)));

        this->updateCount(Piece::Empty, piece->state());
    }
}

Piece* Board::piece(int r, int c) const {
    return (r >= 0 && r < this->rows() &&
            c >= 0 && c < this->cols()) ? m_board[r][c] : 0;
}

bool Board::isPlayable(const Board::Play play) const {
    return play.from && (play.from->state() == Piece::Filled || play.from->state() == Piece::Selected) &&
           play.over && play.over->state() == Piece::Filled &&
           play.to && (play.to->state() == Piece::Empty || play.to->state() == Piece::Jumpable);
}

QList<Board::Play> Board::plays(Piece* piece) const {
    QList<Board::Play> plays;

    if (piece && piece->state() == Piece::Filled) {
        Board::Play p;
        p.from = piece;

        int r = piece->row();
        int c = piece->col();

        foreach (Board::Direction d,
                    QList<Board::Direction>() <<
                        Board::Up << Board::Right << Board::Down << Board::Left) {
            switch (d) {
                case Board::Up:
                    p.over = this->piece(r-1, c);
                    p.to = this->piece(r-2, c);
                    break;
                case Board::Right:
                    p.over = this->piece(r, c+1);
                    p.to = this->piece(r, c+2);
                    break;
                case Board::Down:
                    p.over = this->piece(r+1, c);
                    p.to = this->piece(r+2, c);
                    break;
                case Board::Left:
                    p.over = this->piece(r, c-1);
                    p.to = this->piece(r, c-2);
                    break;
                default:
                    break;
            }

            if ((p.over && p.over->state() == Piece::Filled) &&
                (p.to && p.to->state() == Piece::Empty))
                plays << p;
        }
    }

    return plays;
}

void Board::play(const Board::Play play) {
    Q_ASSERT(this->isPlayable(play));

    play.to->setState(Piece::Filled);
    play.over->setState(Piece::Empty);
    play.from->setState(Piece::Empty);

    this->checkGameOver();
}

void Board::updateCount(Piece::State olds, Piece::State news) {
    int oldcount = this->count();

    switch (olds) {
        case Piece::Filled:
            m_filled--;
            break;
        case Piece::Selected:
            m_selected--;
            break;
        default:
            break;
    }

    switch (news) {
        case Piece::Filled:
            m_filled++;
            break;
        case Piece::Selected:
            m_selected++;
            break;
        default:
            break;
    }

    if (oldcount != this->count())
        emit countChanged(this->count());
}

void Board::checkGameOver() {
    for (int r = 0; r < this->rows(); r++) {
        for (int c = 0; c < this->cols(); c++) {
            Piece* piece = m_board[r][c];
            if (piece && piece->state() == Piece::Filled) {
                if (this->plays(piece).count() > 0)
                    return;
            }
        }
    }

    emit gameOver();
}

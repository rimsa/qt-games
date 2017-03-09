#include "Piece.h"

Piece::Piece(QWidget* parent) :
    QPushButton(parent),
    m_row(0), m_col(0),
    m_state(Piece::Empty) {

    this->updatePiece();

    QObject::connect(this, SIGNAL(stateChanged(Piece::State,Piece::State)), this, SLOT(updatePiece()));
}

Piece::~Piece() {
}

void Piece::setRow(int row) {
    if (m_row != row)
        m_row = row;
}

void Piece::setCol(int col) {
    if (m_col != col)
        m_col = col;
}

void Piece::setState(Piece::State state) {
    if (m_state != state) {
        Piece::State old = m_state;

        // change the state.
        m_state = state;

        // notify the state change
        emit stateChanged(old, state);
    }
}

void Piece::updatePiece() {
    // mark the button disabled if empty.
    this->setDisabled(m_state == Piece::Empty);

    // set the pixmap.
    this->setIcon(Piece::stateToPixmap(m_state));
}

QPixmap Piece::stateToPixmap(Piece::State state) {
    switch (state) {
        case Empty:
            return QPixmap(":empty");
        case Filled:
            return QPixmap(":filled");
        case Selected:
            return QPixmap(":selected");
        case Jumpable:
            return QPixmap(":jumpable");
        default:
            return QPixmap();
    }
}

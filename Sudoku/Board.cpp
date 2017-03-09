#include "Board.h"
#include "ui_Board.h"

#include <QMessageBox>
#include <algorithm>
#include "Cell.h"

Board::Board(QWidget *parent) :
    QWidget(parent), m_mode(DefaultBoards::Easy), ui(new Ui::Board) {

    ui->setupUi(this);

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            Cell* cell = this->findChild<Cell*>(QString("cell%1%2").arg(r+1).arg(c+1));
            Q_ASSERT_X(cell, "board", "invalid cell");
            Q_ASSERT_X(!m_all.contains(cell), "board", "duplicated cell");

            cell->setPosition(Position(r, c));
            QObject::connect(cell, SIGNAL(valueChanged(int)), this, SLOT(updateValue()));
            QObject::connect(cell, SIGNAL(valueChanged(int)), this, SLOT(checkBoard()));

            m_cells[r][c] = cell;
            m_all << cell;
        }
    }

    QObject::connect(this, SIGNAL(modeChanged(DefaultBoards::Mode)), this, SLOT(renew()));
    QObject::connect(this, SIGNAL(gameOver()), this, SLOT(showCongratulations()));

    QMetaObject::invokeMethod(this, "renew");
}

Board::~Board() {
    delete ui;
}

void Board::renew() {
    this->clear();

    DefaultBoards::SimpleBoard board =
        DefaultBoards::instance()->getBoard(m_mode/*DefaultBoards::Test*/);

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int value = board.board[r][c];
            m_cells[r][c]->setValue(value);
            m_cells[r][c]->setEnabled(value == 0);
        }
    }
}

void Board::clear() {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            Cell* tmp = m_cells[r][c];
            if (tmp->isEnabled())
                m_cells[r][c]->setValue(0);
        }
    }
}

void Board::setMode(DefaultBoards::Mode mode) {
    if (mode != m_mode) {
        m_mode = mode;
        emit modeChanged(mode);
    }
}

bool Board::checkLine(Cell* cell) const {
    int r = cell->row();
    for (int c = 0; c < 9; c++) {
        Cell* tmp = m_cells[r][c];
        if (this->checkCell(cell, tmp))
            return false;
    }

    return true;
}

bool Board::checkColumn(Cell* cell) const {
    int c = cell->col();
    for (int r = 0; r < 9; r++) {
        Cell* tmp = m_cells[r][c];
        if (this->checkCell(cell, tmp))
            return false;
    }

    return true;
}

bool Board::checkBox(Cell* cell) const {
    int i = (cell->row() / 3) * 3;
    int j = (cell->col() / 3) * 3;

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            Cell* tmp = m_cells[i+r][j+c];
            if (this->checkCell(cell, tmp))
                return false;
        }
    }

    return true;
}

bool Board::checkCell(Cell* orig, Cell* other) const {
    return (orig != other && // don't compare the value of the current cell.
            !other->isInvalid() && // only compare valid values.
            orig->value() == other->value()); // check if the value is the same.
}

void Board::checkBoard() {
    struct {
        int valid;
        int invalid;
    } count = { 0, 0 };

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (m_cells[r][c]->isInvalid())
                count.invalid++;
            else if (m_cells[r][c]->isEnabled())
                count.valid++;
        }
    }

    emit completed((count.valid * 100.0) / (count.valid + count.invalid));

    if (count.invalid == 0)
        emit gameOver();
}

void Board::updateValue() {
    Cell* cell = qobject_cast<Cell*>(QObject::sender());
    if (!cell)
        return;

    if (cell->value() == 0) {
        cell->setInvalid(true);
        return;
    }

    bool ok = checkLine(cell) && checkColumn(cell) && checkBox(cell);
    cell->setInvalid(!ok);
}

void Board::showCongratulations() {
    QMessageBox::information(
        this,
        tr("Fim do jogo"),
        tr("Parabéns, você completou o tabuleiro."));
}


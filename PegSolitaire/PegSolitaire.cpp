#include "PegSolitaire.h"
#include "ui_PegSolitaire.h"

#include <QList>
#include <QPair>
#include <QDebug>
#include <QActionGroup>
#include <QSignalMapper>
#include <QMessageBox>

PegSolitaire::PegSolitaire(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PegSolitaire),
    m_mode(PegSolitaire::Standard),
    m_board(new Board(parent)) {

    ui->setupUi(this);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(ui->actionTradicional);
    group->addAction(ui->actionCruz);
    group->addAction(ui->actionMais);
    group->addAction(ui->actionBanquinho);
    group->addAction(ui->actionFlecha);
    group->addAction(ui->actionPiramide);
    group->addAction(ui->actionLosango);

    QObject::connect(ui->actionNovo, SIGNAL(triggered()), this, SLOT(fill()));
    QObject::connect(ui->actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(group, SIGNAL(triggered(QAction*)), this, SLOT(updateMode(QAction*)));
    QObject::connect(ui->actionSobre, SIGNAL(triggered()), this, SLOT(showAbout()));

    // Load the pieces.
    QSignalMapper* map = new QSignalMapper(this);
    for (int r = 0; r < m_board->rows(); r++) {
        for (int c = 0; c < m_board->cols(); c++) {
            // Find the piece and map clicked signal.
            Piece* piece = this->findChild<Piece*>(QString("peca%1%2").arg(r).arg(c));
            if (piece) {
                m_board->addPiece(r, c, piece);
                map->setMapping(piece, ((r * m_board->rows()) + c));
                QObject::connect(piece, SIGNAL(clicked()), map, SLOT(map()));
            }
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(checkPlay(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(checkPlay(int)));
#endif

    QObject::connect(m_board, SIGNAL(countChanged(int)), this, SLOT(showStatus(int)));
    QObject::connect(m_board, SIGNAL(gameOver()), this, SLOT(showGameOver()));

    // Fill the board with the current mode.
    this->fill();

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

PegSolitaire::~PegSolitaire() {
    delete ui;
}

void PegSolitaire::setMode(PegSolitaire::Mode mode) {
    if (m_mode != mode) {
        // set the mode
        m_mode = mode;

        // emit the signal
        emit modeChanged(mode);
    }
}

void PegSolitaire::clear() {
    for (int r = 0; r < m_board->rows(); r++) {
        for (int c = 0; c < m_board->cols(); c++) {
            Piece* piece = m_board->piece(r, c);
            if (piece)
                piece->setState(Piece::Empty);
        }
    }
}

void PegSolitaire::fill() {
    // clear the board.
    this->clear();

    // fill the board.
    switch (m_mode) {
        case Standard:
            fillStandard();
            break;
        case Cross:
            fillCross();
            break;
        case Plus:
            fillPlus();
            break;
        case Bench:
            fillBench();
            break;
        case Arrow:
            fillArrow();
            break;
        case Pyramid:
            fillPyramid();
            break;
        case Diamond:
            fillDiamond();
            break;
        default:
            break;
    }

    // Reset moves if any.
    m_moves.clear();

    // Update the status bar.
    this->showStatus(m_board->count());
}

void PegSolitaire::fillStandard() {
    for (int r = 0; r < m_board->rows(); r++) {
        for (int c = 0; c < m_board->cols(); c++) {
            Piece* piece = m_board->piece(r, c);
            if (piece)
                piece->setState(Piece::Filled);
        }
    }

    Piece* middle = m_board->piece(3, 3);
    Q_ASSERT(middle != 0);
    middle->setState(Piece::Empty);
}

void PegSolitaire::fillCross() {
    m_board->piece(1, 3)->setState(Piece::Filled);
    m_board->piece(2, 2)->setState(Piece::Filled);
    m_board->piece(2, 3)->setState(Piece::Filled);
    m_board->piece(2, 4)->setState(Piece::Filled);
    m_board->piece(3, 3)->setState(Piece::Filled);
    m_board->piece(4, 3)->setState(Piece::Filled);
}

void PegSolitaire::fillPlus() {
    for (int r = 1; r < m_board->rows()-1; r++) {
        Piece* piece = m_board->piece(r, 3);
        Q_ASSERT(piece != 0);
        piece->setState(Piece::Filled);
    }

    for (int c = 1; c < m_board->cols()-1; c++) {
        Piece* piece = m_board->piece(3, c);
        Q_ASSERT(piece != 0);
        piece->setState(Piece::Filled);
    }
}

void PegSolitaire::fillBench() {
    for (int r = 0; r <= 3; r++) {
        for (int c = 2; c <= 4; c++) {
            Piece* piece = m_board->piece(r, c);
            Q_ASSERT(piece != 0);
            piece->setState(Piece::Filled);
        }
    }

    Piece* middle = m_board->piece(3, 3);
    Q_ASSERT(middle != 0);
    middle->setState(Piece::Empty);
}

void PegSolitaire::fillArrow() {
    m_board->piece(0, 3)->setState(Piece::Filled);
    m_board->piece(1, 2)->setState(Piece::Filled);
    m_board->piece(1, 3)->setState(Piece::Filled);
    m_board->piece(1, 4)->setState(Piece::Filled);
    m_board->piece(2, 1)->setState(Piece::Filled);
    m_board->piece(2, 2)->setState(Piece::Filled);
    m_board->piece(2, 3)->setState(Piece::Filled);
    m_board->piece(2, 4)->setState(Piece::Filled);
    m_board->piece(2, 5)->setState(Piece::Filled);
    m_board->piece(3, 3)->setState(Piece::Filled);
    m_board->piece(4, 3)->setState(Piece::Filled);
    m_board->piece(5, 2)->setState(Piece::Filled);
    m_board->piece(5, 3)->setState(Piece::Filled);
    m_board->piece(5, 4)->setState(Piece::Filled);
    m_board->piece(6, 2)->setState(Piece::Filled);
    m_board->piece(6, 3)->setState(Piece::Filled);
    m_board->piece(6, 4)->setState(Piece::Filled);
}

void PegSolitaire::fillPyramid() {
    for (int r = 1; r <= 4; r++) {
        int t = ((r-1) * 2) + 1;
        int offset = (3 - (t / 2));

        for (int c = 0; c < t; c++) {
            Piece* piece = m_board->piece(r, (c + offset));
            Q_ASSERT(piece != 0);
            piece->setState(Piece::Filled);
        }
    }
}

void PegSolitaire::fillDiamond() {
    for (int r = 0; r < m_board->rows(); r++) {
        for (int c = 0; c < m_board->cols(); c++) {
            Piece* piece = m_board->piece(r, c);
            if (piece)
                piece->setState(Piece::Filled);
        }
    }

    m_board->piece(0, 2)->setState(Piece::Empty);
    m_board->piece(0, 4)->setState(Piece::Empty);
    m_board->piece(2, 0)->setState(Piece::Empty);
    m_board->piece(2, 6)->setState(Piece::Empty);
    m_board->piece(3, 3)->setState(Piece::Empty);
    m_board->piece(4, 0)->setState(Piece::Empty);
    m_board->piece(4, 6)->setState(Piece::Empty);
    m_board->piece(6, 2)->setState(Piece::Empty);
    m_board->piece(6, 4)->setState(Piece::Empty);
}

void PegSolitaire::checkPlay(int pos) {
    int r = pos / m_board->rows();
    int c = pos % m_board->rows();

    Piece* current = m_board->piece(r, c);

    if (m_moves.count() > 1) {
        if (current->state() == Piece::Jumpable) {
            foreach (Board::Play p, m_moves) {
                // We are jumping to this place.
                if (p.to == current)
                    m_board->play(p);
                // Otherwise, reset.
                else
                    p.to->setState(Piece::Empty);
            }

            m_moves.clear();
        }
    } else {
        m_moves = m_board->plays(current);
        switch (m_moves.count()) {
            case 0:
                // Cannot be played, just ignore.
                break;
            case 1:
                // A single play? Just do it.
                m_board->play(m_moves[0]);
                m_moves.clear();
                break;
            default:
                // Multiple movements.
                current->setState(Piece::Selected);
                foreach (Board::Play p, m_moves)
                    p.to->setState(Piece::Jumpable);

                break;
        }
    }
}

void PegSolitaire::updateMode(QAction* action) {
    if (action == ui->actionTradicional)
        m_mode = PegSolitaire::Standard;
    else if (action == ui->actionCruz)
        m_mode = PegSolitaire::Cross;
    else if (action == ui->actionMais)
        m_mode = PegSolitaire::Plus;
    else if (action == ui->actionBanquinho)
        m_mode = PegSolitaire::Bench;
    else if (action == ui->actionFlecha)
        m_mode = PegSolitaire::Arrow;
    else if (action == ui->actionPiramide)
        m_mode = PegSolitaire::Pyramid;
    else if (action == ui->actionLosango)
        m_mode = PegSolitaire::Diamond;

    this->fill();
}

void PegSolitaire::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Resta Um\n\nAndrei Rimsa Alvares - andrei@decom.cefetmg.br"));
}

void PegSolitaire::showStatus(int count) {
    ui->statusBar->showMessage(QString("Peças remanescentes: %1").arg(count));
}

void PegSolitaire::showGameOver() {
    QString msg = m_board->count() == 1 ?
                    tr("Parabéns, você terminou o jogo") :
                    tr("Não existem mais jogadas possíveis");

    QMessageBox::information(this, tr("Fim do jogo"), msg);
}

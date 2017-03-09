#ifndef PEGSOLITAIRE_H
#define PEGSOLITAIRE_H

#include <QList>
#include <QMainWindow>

#include <Piece.h>
#include <Board.h>

namespace Ui {
    class PegSolitaire;
}

class PegSolitaire : public QMainWindow {
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY(PegSolitaire::Mode mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    enum Mode {
        Standard,
        Cross,
        Plus,
        Bench,
        Arrow,
        Pyramid,
        Diamond
    };

    explicit PegSolitaire(QWidget *parent = 0);
    virtual ~PegSolitaire();

    PegSolitaire::Mode mode() const { return m_mode; }
    void setMode(PegSolitaire::Mode mode);

public slots:
    void clear();
    void fill();

signals:
    void modeChanged(PegSolitaire::Mode mode);

private:
    Ui::PegSolitaire *ui;
    PegSolitaire::Mode m_mode;
    Board* m_board;
    QList<Board::Play> m_moves;

    void fillStandard();
    void fillCross();
    void fillPlus();
    void fillBench();
    void fillArrow();
    void fillPyramid();
    void fillDiamond();

private slots:
    void checkPlay(int pos);

    void updateMode(QAction* action);
    void showAbout();
    void showStatus(int count);
    void showGameOver();

};

#endif // PEGSOLITAIRE_H

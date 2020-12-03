#ifndef NINEHOLES_H
#define NINEHOLES_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NineHoles; }
QT_END_NAMESPACE

class Hole;

class NineHoles : public QMainWindow {
    Q_OBJECT

public:
    enum Player {
        RedPlayer,
        BluePlayer
    };
    Q_ENUM(Player)

    enum Phase {
        DropPhase,
        MovePhase
    };
    Q_ENUM(Phase)

    NineHoles(QWidget *parent = nullptr);
    virtual ~NineHoles();

    Hole* holeAt(int row, int col) const;

signals:
    void gameOver(Player player);

private:
    Ui::NineHoles *ui;
    Hole* m_holes[9];
    Player m_player;
    Phase m_phase;
    int m_dropCount;
    Hole* m_selected;

    void drop(Hole* hole);
    void move(Hole* hole);

    void switchPlayer();
    void clearSelectable();
    QList<Hole*> findSelectable(Hole* hole);

    bool checkRow(Player player, int col);
    bool checkCol(Player player, int row);
    bool isGameOver(Hole* hole);

private slots:
    void play(int id);
    void reset();
    void showAbout();
    void showGameOver(Player player);
    void updateStatusBar();

};

#endif // NINEHOLES_H

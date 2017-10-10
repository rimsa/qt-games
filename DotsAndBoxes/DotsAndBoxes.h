#ifndef DOTSANDBOXES_H
#define DOTSANDBOXES_H

#include <QMainWindow>

namespace Ui {
    class DotsAndBoxes;
}

class Player;
class BoxLabel;
class LineButton;

class DotsAndBoxes : public QMainWindow {
    Q_OBJECT

public:
    explicit DotsAndBoxes(QWidget *parent = 0);
    virtual ~DotsAndBoxes();

private:
    Ui::DotsAndBoxes *ui;
    int m_playerid;
    bool m_keepPlayer;
    bool m_gameOver;
    Player* m_players[2];
    LineButton* m_lines[7][4];
    BoxLabel* m_boxes[3][3];

private slots:
    void reset();
    void changeLine();
    void keepPlayer();
    void showAbout();
    void updateStatus();
    void showGameOver();

};

#endif // DOTSANDBOXES_H

#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class Player;

class Cell : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(bool inverted READ isInverted WRITE setInverted NOTIFY invertedChanged)

public:
    explicit Cell(QWidget *parent = nullptr);
    virtual ~Cell();

    bool isInverted() const { return m_inverted; }
    void setInverted(bool inverted);

    Player* player() const { return m_player; }
    void setPlayer(Player* player);

public slots:
    void reset();

signals:
    void playerChanged(Player* player);
    void invertedChanged(bool inverted);

private:
    Player* m_player;
    bool m_inverted;

private slots:
    void updateCell();

};

#endif // CELL_H

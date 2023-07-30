#ifndef HOLE_H
#define HOLE_H

#include <QObject>
#include <QPushButton>

#include "Player.h"

class Hole : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(int col READ col WRITE setCol)
    Q_PROPERTY(Hole::State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_PROPERTY(bool playable READ isPlayable)
    Q_PROPERTY(bool used READ isUsed)
    Q_PROPERTY(bool selected READ isSelected)

public:
    enum State {
        Empty,
        Playable,
        Used,
        Selected
    };
    Q_ENUM(State)

    explicit Hole(QWidget *parent = nullptr);
    virtual ~Hole();

    int row() const { return m_row; }
    void setRow(int row) { m_row = row; }

    int col() const { return m_col; }
    void setCol(int col) { m_col = col; }

    Hole::State state() const { return m_state; }
    void setState(Hole::State state);

    Player* player() const { return m_player; }
    void setPlayer(Player* player);

    bool isEmpty() const { return m_state == State::Empty; }
    bool isPlayable() const { return m_state == State::Playable; }
    bool isUsed() const { return m_state == State::Used; }
    bool isSelected() const { return m_state == State::Selected; }

public slots:
    void reset();

signals:
    void stateChanged(Hole::State state);
    void playerChanged(Player* player);

private:
    int m_row, m_col;
    Hole::State m_state;
    Player* m_player;

private slots:
    void updateHole();

};

#endif // HOLE_H

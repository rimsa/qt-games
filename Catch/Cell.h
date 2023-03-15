#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPushButton>

#include "Player.h"

class Cell : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(int col READ col WRITE setCol)
    Q_PROPERTY(Cell::State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_PROPERTY(bool selectable READ isSelectable)
    Q_PROPERTY(bool blocked READ isBlocked)
    Q_PROPERTY(bool captured READ isCaptured)

public:
    enum State {
        Empty,
        Selectable,
        Blocked,
        Captured
    };
    Q_ENUM(State)

    explicit Cell(QWidget *parent = nullptr);
    virtual ~Cell();

    int row() const { return m_row; }
    void setRow(int row) { m_row = row; }

    int col() const { return m_col; }
    void setCol(int col) { m_col = col; }

    Cell::State state() const { return m_state; }
    void setState(Cell::State state);

    Player* player() const { return m_player; }
    void setPlayer(Player* player);

    bool isEmpty() const { return m_state == State::Empty; }
    bool isSelectable() const { return m_state == State::Selectable; }
    bool isBlocked() const { return m_state == State::Blocked; }
    bool isCaptured() const { return m_state == State::Captured; }

public slots:
    void reset();

signals:
    void mouseOver(bool over);
    void stateChanged(Cell::State state);
    void playerChanged(Player* player);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    virtual void enterEvent(QEvent *event);
#else
    virtual void enterEvent(QEnterEvent *event);
#endif
    virtual void leaveEvent(QEvent *event);

private:
    int m_row, m_col;
    Cell::State m_state;
    Player* m_player;

private slots:
    void updateCell();

};

#endif // CELL_H

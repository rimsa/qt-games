#ifndef HOLE_H
#define HOLE_H

#include <QObject>
#include <QPushButton>

class Hole : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(int col READ col WRITE setCol)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool marked READ isMarked WRITE setMarked NOTIFY markedChanged)

public:
    enum State {
        EmptyState,
        RedState,
        BlueState,
    };
    Q_ENUM(State)

    enum Direction {
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest
    };
    Q_ENUM(Direction)

    explicit Hole(QWidget *parent = nullptr);
    virtual ~Hole();

    int row() const { return m_row; }
    void setRow(int row) { m_row = row; }

    int col() const { return m_col; }
    void setCol(int col) { m_col = col; }

    State state() const { return m_state; }
    void setState(State State);

    bool isMarked() const { return m_marked; }
    void setMarked(bool marked);

    QList<Hole::Direction> moves();
    static QList<Hole::Direction> allDirections();

public slots:
    void reset();

signals:
    void stateChanged(Hole::State State);
    void markedChanged(bool marked);

private:
    int m_row, m_col;
    State m_state;
    bool m_marked;

private slots:
    void updateHole();

};

#endif // HOLE_H

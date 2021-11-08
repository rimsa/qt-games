#ifndef HOLE_H
#define HOLE_H

#include <QObject>
#include <QPushButton>

class Hole : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
    enum State {
        EmptyState,
        RedState,
        BlueState,
        SelectableState
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

    State state() const { return m_state; }
    void setState(State State);

    Hole* neighbor(Hole::Direction direction) const { return m_neighbors[direction]; }
    void setNeighbor(Hole::Direction direction, Hole* hole) { m_neighbors[direction] = hole; }

    static QList<Hole::Direction> directions();

public slots:
    void reset();

signals:
    void stateChanged(State State);

private:
    State m_state;
    Hole* m_neighbors[8];

    static QPixmap stateToPixmap(State state);

private slots:
    void updateHole(State state);

};

#endif // HOLE_H

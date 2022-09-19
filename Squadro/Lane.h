#ifndef LANE_H
#define LANE_H

#include "Squadro.h"

#include <QObject>
#include <QList>

class Cell;

class Lane : public QObject {
    Q_OBJECT

public:
    enum Direction {
        Forward,
        Backward
    };
    Q_ENUM(Direction)

    explicit Lane(Player* player, const QList<Cell*>& cells,
        int going, int returning, QObject *parent = nullptr);
    virtual ~Lane();

    Cell* home() const;
    Cell* end() const;
    Cell* current() const;

    bool contains(Cell* cell);
    bool isFinished();

public slots:
    void reset();
    void move();
    void sendBack();

signals:
    void finished();

private:
    Player* m_player;
    QList<Cell*> m_cells;
    struct {
        int forward;
        int backward;
    } m_steps;
    Direction m_direction;
    int m_current;

};

#endif // LANE_H

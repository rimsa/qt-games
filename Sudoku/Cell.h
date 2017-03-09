#ifndef CELL_H
#define CELL_H

#include <QSpinBox>
#include "Position.h"

class Cell : public QSpinBox {
    Q_OBJECT
    Q_PROPERTY(bool invalid READ isInvalid WRITE setInvalid NOTIFY invalidChanged)

public:
    explicit Cell(QWidget *parent = 0);
    explicit Cell(const Position& position, QWidget *parent = 0);

    virtual ~Cell();

    const Position& position() { return m_position; }
    int row() const { return m_position.row(); }
    int col() const { return m_position.col(); }

    bool isInvalid() const { return m_invalid; }

signals:
    void invalidChanged(bool invalid);

public slots:
    void setPosition(const Position position);
    void setInvalid(bool invalid = true);

private:
    Position m_position;
    bool m_invalid;

private slots:
    void repolish();

};

#endif // CELL_H

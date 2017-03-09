#include <QStyle>
#include "Cell.h"

Cell::Cell(QWidget *parent) : QSpinBox(parent), m_invalid(true) {
    this->setSpecialValueText(" ");

    QObject::connect(this, SIGNAL(invalidChanged(bool)), this, SLOT(repolish()));
}

Cell::Cell(const Position& position, QWidget *parent)
    : QSpinBox(parent), m_position(position), m_invalid(true) {
    this->setSpecialValueText(" ");

    QObject::connect(this, SIGNAL(invalidChanged(bool)), this, SLOT(repolish()));
}

Cell::~Cell() {
}

void Cell::setPosition(const Position position) {
    m_position = position;
}

void Cell::setInvalid(bool invalid) {
    if (m_invalid != invalid) {
        m_invalid = invalid;

        emit invalidChanged(invalid);
    }
}

void Cell::repolish() {
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

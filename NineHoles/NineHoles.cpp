#include "NineHoles.h"
#include "ui_NineHoles.h"

#include <QDebug>
#include <QSignalMapper>
#include <QMessageBox>

NineHoles::NineHoles(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::NineHoles) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 9; id++) {
        int r = id / 3;
        int c = id % 3;

        Hole* hole = this->findChild<Hole*>(QString("hole%1%2").arg(r).arg(c));
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));
    }
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));

    this->adjustSize();
    this->setFixedSize(this->size());
}

NineHoles::~NineHoles() {
    delete ui;
}

void NineHoles::play(int id) {
    Hole* hole = m_holes[id];

    qDebug() << "row: " << hole->row();
    qDebug() << "col: " << hole->col();

    hole->setState(Hole::RedState);
}

void NineHoles::reset() {
    for (int id = 0; id < 9; id++) {
        Hole* hole = m_holes[id];
        hole->setState(Hole::EmptyState);
    }
}

void NineHoles::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Nine Holes\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

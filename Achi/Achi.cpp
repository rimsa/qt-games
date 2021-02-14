#include "Achi.h"
#include "ui_Achi.h"

#include <QMessageBox>
#include <QSignalMapper>

#include <QDebug>

Achi::Achi(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Achi) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    // Load the pieces.
    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 9; ++id) {
        int r = id / 3;
        int c = id % 3;

        Hole* hole = this->findChild<Hole*>(QString("hole%1%2").arg(r).arg(c));
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked()), map, SLOT(map()));
    }
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

Achi::~Achi() {
    delete ui;
}

void Achi::play(int index) {
    Hole* hole = m_holes[index];
    Q_ASSERT(hole != nullptr);

    qDebug() << QString("hole: (%1, %2)").arg(hole->row()).arg(hole->col());
    hole->setState(Hole::RedState);
}

void Achi::reset() {
    for (int id = 0; id < 9; ++id) {
        Hole* hole = m_holes[id];
        Q_ASSERT(hole != nullptr);

        m_holes[id]->reset();
    }
}

void Achi::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Achi"));
}

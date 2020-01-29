#include "Ludo.h"
#include "ui_Ludo.h"

Ludo::Ludo(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Ludo) {
    ui->setupUi(this);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(ui->action2);
    group->addAction(ui->action3);
    group->addAction(ui->action4);

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

Ludo::~Ludo() {
    delete ui;
}

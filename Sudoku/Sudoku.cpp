#include "Sudoku.h"
#include "ui_Sudoku.h"

#include <QActionGroup>
#include <QMessageBox>
#include "Board.h"

Sudoku::Sudoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sudoku) {
    ui->setupUi(this);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(ui->actionFacil);
    group->addAction(ui->actionMedio);
    group->addAction(ui->actionDificil);

    QObject::connect(ui->actionNovo, SIGNAL(triggered()), ui->board, SLOT(renew()));
    QObject::connect(ui->actionLimpar, SIGNAL(triggered()), ui->board, SLOT(clear()));
    QObject::connect(ui->actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(group, SIGNAL(triggered(QAction*)), this, SLOT(changeMode(QAction*)));
    QObject::connect(ui->board, SIGNAL(completed(double)), this, SLOT(showProgress(double)));
    QObject::connect(ui->actionSobre, SIGNAL(triggered()), this, SLOT(showAbout()));

    this->showProgress(0);

    this->adjustSize();
    this->setFixedSize(this->size());
}

Sudoku::~Sudoku() {
    delete ui;
}

void Sudoku::changeMode(QAction* action) {
    DefaultBoards::Mode mode;

    if (action == ui->actionFacil)
        mode = DefaultBoards::Easy;
    else if (action == ui->actionMedio)
        mode = DefaultBoards::Medium;
    else if (action == ui->actionDificil)
        mode = DefaultBoards::Hard;
    else {
        Q_UNREACHABLE();
        qApp->exit();
    }

    ui->board->setMode(mode);
}

void Sudoku::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Sudoku\n\nAndrei Rimsa Alvares - ???????"));
}

void Sudoku::showProgress(double progress) {
    ui->statusBar->showMessage(QString("Progresso: %1%").arg(QString::number(progress, 'f', 2)));
}

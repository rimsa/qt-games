#include "Board.h"
#include "ui_Board.h"

Board::Board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Board) {
    ui->setupUi(this);
}

Board::~Board() {
    delete ui;
}

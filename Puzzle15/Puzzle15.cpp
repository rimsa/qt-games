#include "Puzzle15.h"
#include "ui_Puzzle15.h"

#include <QDebug>
#include <QSignalMapper>
#include <QtGlobal>
#include <QTime>

Puzzle15::Puzzle15(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Puzzle15),
    m_moves(0) {

    ui->setupUi(this);

    qsrand(QTime::currentTime().msec());

    QList<QPushButton*> tiles;
    tiles << ui->tile1 << ui->tile2 << ui->tile3
          << ui->tile4 << ui->tile5 << ui->tile6
          << ui->tile7 << ui->tile8 << ui->tile9;

    int number = 1;
    QSignalMapper* mapper = new QSignalMapper(this);
    foreach (QPushButton* tile, tiles) {
        m_tiles[number] = tile;

        mapper->setMapping(tile, number);
        QObject::connect(tile, SIGNAL(clicked()), mapper, SLOT(map()));

        number++;
    }

    QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(handleTileChange(int)));

    this->updateMovements();

    this->shuffleTiles();
}

Puzzle15::~Puzzle15() {
    delete ui;
}

QPushButton* Puzzle15::top(int tileNumber) const {
    return this->tile(tileNumber - 3);
}

QPushButton* Puzzle15::left(int tileNumber) const {
    return (((tileNumber - 1) % 3) != 0 ?
        this->tile(tileNumber - 1) : NULL);

}

QPushButton* Puzzle15::bottom(int tileNumber) const {
    return this->tile(tileNumber + 3);
}

QPushButton* Puzzle15::right(int tileNumber) const {
    return ((tileNumber % 3) != 0 ?
        this->tile(tileNumber + 1) : NULL);
}

QPushButton* Puzzle15::tile(int tileNumber) const {
    return (m_tiles.contains(tileNumber) ?
        m_tiles[tileNumber] : NULL);
}

bool Puzzle15::canMoveTo(QPushButton* button) const {
    return button ? !button->isEnabled() : false;
}

void Puzzle15::moveTile(QPushButton* from, QPushButton* to) {
    QString tmp = from->text();

    from->setText("");
    from->setEnabled(false);

    to->setText(tmp);
    to->setEnabled(true);

    this->updateMovements();
}

void Puzzle15::handleTileChange(int tileNumber) {
    QPushButton* current = m_tiles[tileNumber];
//    qDebug() << tileNumber << ": " << current;

    QPushButton* top = this->top(tileNumber);
    QPushButton* left = this->left(tileNumber);
    QPushButton* bottom = this->bottom(tileNumber);
    QPushButton* right = this->right(tileNumber);

//    qDebug() << "Top: " << top;
//    qDebug() << "Left: " << left;
//    qDebug() << "Bottom: " << bottom;
//    qDebug() << "Right: " << right;

    if (this->canMoveTo(top))
        this->moveTile(current, top);
    else if (this->canMoveTo(left))
        this->moveTile(current, left);
    else if (this->canMoveTo(bottom))
        this->moveTile(current, bottom);
    else if (this->canMoveTo(right))
        this->moveTile(current, right);
}

void Puzzle15::updateMovements() {
    ui->statusBar->showMessage(QString("Movimentos: %1").arg(m_moves++));
}

void Puzzle15::shuffleTiles() {
    QStringList list;
    for (int i = 1; i < 9; i++)
        list << QString::number(i);
    list << "";

    QStringList shuffle;
    while (list.count() > 0) {
        QString tileName = list[qrand() % list.count()];

        shuffle << tileName;
        list.removeAll(tileName);
    }

    int pos = 1;
    foreach (QString tileName, shuffle) {
        QPushButton* button = m_tiles[pos++];

        button->setText(tileName);
        button->setEnabled(tileName != "");
    }
}

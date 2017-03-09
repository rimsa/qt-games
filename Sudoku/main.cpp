#include "Sudoku.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sudoku w;
    w.show();

    return a.exec();
}

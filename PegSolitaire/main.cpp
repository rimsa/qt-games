#include "PegSolitaire.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PegSolitaire w;
    w.show();

    return a.exec();
}

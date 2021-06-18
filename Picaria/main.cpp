#include "Picaria.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Picaria w;

    w.show();

    return a.exec();
}

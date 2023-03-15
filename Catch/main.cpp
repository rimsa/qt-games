#include "Catch.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Catch w;
    w.show();
    return a.exec();
}

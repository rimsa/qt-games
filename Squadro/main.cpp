#include "Squadro.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Squadro w;
    w.show();
    return a.exec();
}

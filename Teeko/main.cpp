#include "Teeko.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Teeko w;
    w.show();
    return a.exec();
}

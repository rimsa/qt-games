#include "Achi.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Achi w;

    w.show();

    return a.exec();
}

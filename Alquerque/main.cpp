#include "Alquerque.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Alquerque w;

    w.show();

    return a.exec();
}

#include "SlimeTrail.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlimeTrail w;
    w.show();
    return a.exec();
}

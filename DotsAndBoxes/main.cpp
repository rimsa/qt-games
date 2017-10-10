#include <QApplication>
#include <DotsAndBoxes.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DotsAndBoxes w;
    w.show();

    return a.exec();
}

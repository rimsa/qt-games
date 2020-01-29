#ifndef LUDO_H
#define LUDO_H

#include <QMainWindow>

namespace Ui {
    class Ludo;
}

class Ludo : public QMainWindow {
    Q_OBJECT

public:
    explicit Ludo(QWidget *parent = 0);
    virtual ~Ludo();

private:
    Ui::Ludo *ui;

};

#endif // LUDO_H

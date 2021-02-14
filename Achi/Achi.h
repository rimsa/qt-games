#ifndef ACHI_H
#define ACHI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Achi;
}
QT_END_NAMESPACE

class Hole;

class Achi : public QMainWindow {
    Q_OBJECT

public:
    Achi(QWidget *parent = nullptr);
    virtual ~Achi();

private:
    Ui::Achi *ui;
    Hole* m_holes[9];

private slots:
    void play(int id);
    void reset();
    void showAbout();

};

#endif // ACHI_H

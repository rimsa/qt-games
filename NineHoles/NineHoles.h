#ifndef NINEHOLES_H
#define NINEHOLES_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NineHoles; }
QT_END_NAMESPACE

class Hole;

class NineHoles : public QMainWindow {
    Q_OBJECT

public:
    NineHoles(QWidget *parent = nullptr);
    virtual ~NineHoles();

private:
    Ui::NineHoles *ui;
    Hole* m_holes[9];

private slots:
    void play(int id);
    void reset();
    void showAbout();

};

#endif // NINEHOLES_H

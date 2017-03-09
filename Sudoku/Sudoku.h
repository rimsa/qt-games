#ifndef SUDOKU_H
#define SUDOKU_H

#include <QMainWindow>

namespace Ui {
    class Sudoku;
}

class Sudoku : public QMainWindow {
    Q_OBJECT

public:
    explicit Sudoku(QWidget *parent = 0);
    virtual ~Sudoku();

public slots:
    void changeMode(QAction* action);
    void showAbout();
    void showProgress(double progress);

private:
    Ui::Sudoku *ui;

};

#endif // SUDOKU_H

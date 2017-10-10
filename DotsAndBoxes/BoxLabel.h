#ifndef BOXLABEL_H
#define BOXLABEL_H

#include <QList>
#include <QLabel>

class Player;
class LineButton;

class BoxLabel : public QLabel {
    Q_OBJECT

public:
    explicit BoxLabel(QWidget *parent = 0);
    virtual ~BoxLabel();

public slots:
    void addLine(LineButton* button);
    void reset();

signals:
    void boxMarked();

private:
    int m_count;
    QList<LineButton*> m_buttons;

private slots:
    void updateCount(Player* player);

};

#endif // BOXLABEL_H

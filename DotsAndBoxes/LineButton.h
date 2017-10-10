#ifndef LINEBUTTON_H
#define LINEBUTTON_H

#include <QPushButton>

class Player;

class LineButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerSet)

public:
    explicit LineButton(QWidget *parent = 0);
    virtual ~LineButton();

    Qt::Orientation mode() const { return m_mode; }
    Player* player() const { return m_player; }

public slots:
    void setMode(Qt::Orientation mode);
    void setPlayer(Player* player);
    void reset();

signals:
    void modeChanged(Qt::Orientation mode);
    void playerSet(Player* player);

private:
    Qt::Orientation m_mode;
    Player* m_player;

private slots:
    void updateStyleSheet();

};

#endif // LINEBUTTON_H

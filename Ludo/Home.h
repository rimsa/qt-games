#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <Player.h>

namespace Ui {
    class Home;
}

class Home : public QWidget {
    Q_OBJECT
    Q_PROPERTY(Player::Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    explicit Home(QWidget *parent = 0);
    virtual ~Home();

    Player::Type type() const;
    void setType(Player::Type type);

    bool active() const;
    void setActive(bool active);

public slots:
    void reset();

signals:
    void typeChanged(Player::Type type);
    void activeChanged(bool active);

private:
    Ui::Home *ui;
    Player* m_player;
    bool m_active;

};

#endif // HOME_H

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QColor>
#include <QPixmap>

class Player : public QObject {
    Q_OBJECT

public:
    enum Type { Empty, Red, Yellow, Green, Blue };
    Q_ENUMS(Type)

    virtual ~Player();

    static Player* player(Player::Type type);

signals:

public:
    Player::Type type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QColor& color() const { return m_color; }
    const QPixmap& pixmap() const { return m_pixmap; }

public slots:

private:
    static Player* m_players[5];
    static QMutex& m_mutex;

    Player::Type m_type;
    QString m_name;
    QColor m_color;
    QPixmap m_pixmap;

    explicit Player(Player::Type type);
    Q_DISABLE_COPY(Player);

};

#endif // PLAYER_H

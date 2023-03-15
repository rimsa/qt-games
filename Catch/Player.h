#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QPixmap>

class Player : public QObject {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Type {
        Red,
        Blue
    };
    Q_ENUM(Type)

    enum Orientation {
        Vertical,
        Horizontal
    };
    Q_ENUM(Orientation)

    virtual ~Player();
    static Player* player(Player::Type type);

    Player::Type type() const { return m_type; }
    Player::Orientation orientation() { return m_orientation; }
    const QString& name() const { return m_name; }
    const QPixmap& pixmap() const { return m_pixmap; }

    int count() const { return m_count; }

    Player* other() const;

public slots:
    void reset();
    void incrementCount();

signals:
    void countChanged(int count);

private:
    static Player* m_players[2];
    static QMutex& m_mutex;

    Player::Type m_type;
    Player::Orientation m_orientation;
    QString m_name;
    QPixmap m_pixmap;
    int m_count;

    explicit Player(Player::Type type);
    Q_DISABLE_COPY(Player);

};

#endif // PLAYER_H

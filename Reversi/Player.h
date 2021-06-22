#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QColor>
#include <QMutex>
#include <QPixmap>

class Player : public QObject {
    Q_OBJECT

public:
    enum Type { Red, Blue };
    Q_ENUMS(Type)

    virtual ~Player();

    static Player* player(Player::Type type);

signals:
    void countChanged(int count);

public:
    Player::Type type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QPixmap& pixmap() const { return m_pixmap; }
    int count() const { return m_count; }

    Player* other() const;

public slots:
    void inc();
    void dec();

private:
    static Player* m_players[2];
    static QMutex& m_mutex;

    Player::Type m_type;
    QString m_name;
    QColor m_color;
    QPixmap m_pixmap;
    int m_count;

    explicit Player(Player::Type type);
    Q_DISABLE_COPY(Player);

};

#endif // PLAYER_H

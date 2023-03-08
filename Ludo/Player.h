#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QColor>
#include <QMutex>
#include <QPixmap>

class Pawn;
class Home;
class Track;
class Status;

class Player : public QObject {
    Q_OBJECT

public:
    enum Type { Empty, Red, Yellow, Green, Blue };
    Q_ENUM(Type)

    virtual ~Player();

    static Player* player(Player::Type type);

signals:

public:
    Player::Type type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QColor& color() const { return m_color; }

    Pawn* pawn(int id) const;
    const QList<Pawn*>& pawns() const;

    Home* home() const { return m_home; }
    Track* track() const { return m_track; }
    Status* status() const { return m_status; }

public slots:
    void reset();

private:
    static Player* m_players[5];
    static QMutex& m_mutex;

    Player::Type m_type;
    QString m_name;
    QColor m_color;
    QList<Pawn*> m_paws;
    QPixmap m_pixmap[2];
    Home* m_home;
    Track* m_track;
    Status* m_status;

    explicit Player(Player::Type type);
    Q_DISABLE_COPY(Player);

    friend class Pawn;
    friend class Home;
    friend class Track;
    friend class Status;

};

#endif // PLAYER_H

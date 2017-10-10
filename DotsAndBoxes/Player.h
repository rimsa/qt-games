#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QColor>

class Player : public QObject {
    Q_OBJECT

public:
    explicit Player(const QString& name, const QColor& color, QObject* parent = 0);
    virtual ~Player();

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    int count() const { return m_count; }

public slots:
    void reset();
    void increaseCount();

signals:
    void countChanged(int count);
    void gameOver();

private:
    static int next_id;
    int m_id;
    QString m_name;
    QColor m_color;
    int m_count;

};

#endif // PLAYER_H

#ifndef BALL_H
#define BALL_H

#include <QMap>
#include <QLabel>
#include <QPixmap>

class Ball : public QLabel {
    Q_OBJECT
    Q_ENUMS(Color)
    Q_PROPERTY(Ball::Color color READ color WRITE setColor NOTIFY colorChanged)

public:
    enum Color {
        BLANK,
        RED,
        BLUE
    };

    Ball(QWidget* parent = 0);
    virtual ~Ball();

    inline Ball::Color color() const { return m_color; }
    inline bool isBlank() const { return m_color == Ball::BLANK; }
    inline bool isRed() const { return m_color == Ball::RED; }
    inline bool isBlue() const { return m_color == Ball::BLUE; }

    static QPixmap colorToPixmap(Ball::Color color);
    static QString colorToString(Ball::Color color);

public slots:
    // Change the color of the ball.
    void setColor(Ball::Color color);

    // Change the color of the ball to blank
    // and does not emit a signal.
    void reset();

signals:
    void colorChanged(Ball::Color color);

private:
    Ball::Color m_color;

};

#endif // BALL_H

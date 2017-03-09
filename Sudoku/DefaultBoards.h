#ifndef DEFAULTBOARDS_H
#define DEFAULTBOARDS_H

#include <QMap>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QSettings>

class DefaultBoards : public QObject {
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode { Test, Easy, Medium, Hard, Evil };
    struct SimpleBoard {
        int board[9][9];
    };

    virtual ~DefaultBoards();
    SimpleBoard getBoard(Mode mode);

    static DefaultBoards* instance();
    static QString mode2str(DefaultBoards::Mode mode);

signals:

public slots:

private:
    static const int BOARDS = 100;

    static DefaultBoards* m_instance;
    static QMutex& m_mutex;
    QSettings m_settings;

    DefaultBoards();

};

#endif // DEFAULTBOARDS_H

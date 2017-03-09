#include "DefaultBoards.h"
#include <ctime>

DefaultBoards* DefaultBoards::m_instance = 0;
QMutex& DefaultBoards::m_mutex = *(new QMutex()); // ugly hack

DefaultBoards::DefaultBoards()
    : QObject(0), m_settings(":/boards", QSettings::IniFormat) {
    std::srand(std::time(0));
}

DefaultBoards::~DefaultBoards() {
}

DefaultBoards::SimpleBoard DefaultBoards::getBoard(DefaultBoards::Mode mode) {
    m_settings.beginGroup(DefaultBoards::mode2str(mode));
    QString boardStr = (mode == DefaultBoards::Test ?
                        m_settings.value("board") :
                        m_settings.value(QString("board%1").arg(std::rand() % BOARDS))).toString();
    m_settings.endGroup();

    Q_ASSERT_X(boardStr.count() == (9*9), "getBoard", "invalid board");

    SimpleBoard board;
    for (int l = 0; l < 9; l++) {
        for (int c = 0; c < 9; c++) {
            board.board[l][c] = boardStr.at((l * 9) + c).digitValue();
        }
    }

    return board;
}

DefaultBoards* DefaultBoards::instance() {
    if (!m_instance) {
        QMutexLocker locker(&m_mutex);
        Q_UNUSED(locker);

        if (!m_instance)
            m_instance = new DefaultBoards();
    }

    return m_instance;
}

QString DefaultBoards::mode2str(DefaultBoards::Mode mode) {
    switch (mode) {
        case DefaultBoards::Test:
            return "Test";
        case DefaultBoards::Easy:
            return "Easy";
        case DefaultBoards::Medium:
            return "Medium";
        case DefaultBoards::Hard:
            return "Hard";
        case DefaultBoards::Evil:
            return "Evil";
        default:
            Q_UNREACHABLE();
    }
}

#include <Connect4.h>
#include "ui_Connect4.h"

#include <QDebug>
#include <QMessageBox>
#include <QSignalMapper>

#include <Game.h>
#include <Ball.h>

Connect4::Connect4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Connect4),
    m_game(0) {

    // Setup the graphical interface.
    ui->setupUi(this);

    // Init game.
    this->initGame();

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

Connect4::~Connect4() {
    delete m_game;
    delete ui;
}

void Connect4::initGame() {
    m_game = new Game(4);
    for (int r = 0; r < m_game->rows(); r++) {
        for (int c = 0; c < m_game->cols(); c++) {
            Ball *ball = this->findChild<Ball*>(QString("row%1%2").arg(r+1).arg(c+1));
            Q_ASSERT(ball != 0);

            m_game->addBall(ball);
        }
    }

    QObject::connect(m_game, SIGNAL(columnFull(int)), this, SLOT(disableColumn(int)));
    QObject::connect(m_game, SIGNAL(victory(Ball::Color, Game::Direction)),
                     this, SLOT(showWinner(Ball::Color, Game::Direction)));

    QSignalMapper* mapper = new QSignalMapper(this);
    for (int c = 0; c < m_game->cols(); c++) {
        // find the button.
        QPushButton *button = this->findChild<QPushButton*>(
            QString("col%1button").arg(c+1));
        Q_ASSERT(button != 0);

        // Added the button to the list.
        m_buttons << button;

        // Set the mapping.
        mapper->setMapping(button, c);
        QObject::connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(mapper, SIGNAL(mapped(int)), m_game, SLOT(play(int)));
#else
    QObject::connect(mapper, SIGNAL(mappedInt(int)), m_game, SLOT(play(int)));
#endif

    QObject::connect(ui->acaoNovo, SIGNAL(triggered()), this, SLOT(reset()));
    QObject::connect(ui->acaoSair, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Connect4::reset() {
    // Reset the game.
    m_game->reset();

    // Enable all buttons.
    foreach (QPushButton* button, m_buttons)
        button->setEnabled(true);
}

void Connect4::disableColumn(int col) {
    Q_ASSERT(col >= 0 && col < m_buttons.count());

    m_buttons[col]->setDisabled(true);

    bool all = true;
    foreach (QPushButton* button, m_buttons) {
        if (button->isEnabled()) {
            all = false;
            break;
        }
    }

    if (all)
        this->showTie();
}

void Connect4::showWinner(Ball::Color color, Game::Direction direction) {
    foreach (QPushButton* button, m_buttons)
        button->setDisabled(true);

    QMessageBox::information(this, tr("Vencedor"),
                                   tr("O jogador %1 venceu na %2")
                                        .arg(Ball::colorToString(color))
                                        .arg(Game::directionToString(direction)));
}

void Connect4::showTie() {
    QMessageBox::information(this, tr("Empate"),
                                   tr("O jogo empatou"));
}

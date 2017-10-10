#include <QMessageBox>

#include <Player.h>
#include <LineButton.h>
#include <DotsAndBoxes.h>
#include "ui_DotsAndBoxes.h"

#include <QDebug>

DotsAndBoxes::DotsAndBoxes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DotsAndBoxes),
    m_playerid(0),
    m_keepPlayer(true),
    m_gameOver(false),
    m_players{new Player("P1", Qt::red), new Player("P2", Qt::blue)} {

    ui->setupUi(this);

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 4; c++) {
            m_lines[r][c] = this->findChild<LineButton*>(QString("line%1%2").arg(r).arg(c));
            if (m_lines[r][c]) {
                QObject::connect(m_lines[r][c], SIGNAL(clicked(bool)), this, SLOT(changeLine()));
            }
        }
    }

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int x = (r * 2) + 1;
            int y = c + 1;

            m_boxes[r][c] = this->findChild<BoxLabel*>(QString("box%1%2").arg(x).arg(y));
            Q_ASSERT(m_boxes[r][c]);

            m_boxes[r][c]->addLine(m_lines[x-1][y-1]);
            m_boxes[r][c]->addLine(m_lines[x][y-1]);
            m_boxes[r][c]->addLine(m_lines[x][y]);
            m_boxes[r][c]->addLine(m_lines[x+1][y-1]);

            QObject::connect(m_boxes[r][c], SIGNAL(boxMarked()), this, SLOT(keepPlayer()));
        }
    }

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionSobre, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    for (int p = 0; p < 2; p++) {
        QObject::connect(m_players[p], SIGNAL(countChanged(int)), this, SLOT(updateStatus()));
        QObject::connect(m_players[p], SIGNAL(gameOver()), this, SLOT(showGameOver()), Qt::QueuedConnection);
    }

    // Update the status bar.
    this->updateStatus();

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

DotsAndBoxes::~DotsAndBoxes() {
    for (int p = 0; p < 2; p++)
        delete m_players[p];

    delete ui;
}

void DotsAndBoxes::reset() {
    m_playerid = 0;
    m_keepPlayer = true;
    m_gameOver = false;

    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 4; c++) {
            if (m_lines[r][c])
                m_lines[r][c]->reset();
        }
    }

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            m_boxes[r][c]->reset();
        }
    }

    for (int p = 0; p < 2; p++)
        m_players[p]->reset();

    this->updateStatus();
}

void DotsAndBoxes::changeLine() {
    LineButton* button = qobject_cast<LineButton*>(QObject::sender());
    if (button) {
        if (m_keepPlayer)
            m_keepPlayer = false;
        else
            m_playerid = (m_playerid + 1) % 2;

        button->setPlayer(m_players[m_playerid % 2]);
    }
}

void DotsAndBoxes::keepPlayer() {
    m_keepPlayer = true;
}

void DotsAndBoxes::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Timbiriche\n\nAndrei Rimsa Alvares - andrei@decom.cefetmg.br"));
}

void DotsAndBoxes::updateStatus() {
    ui->statusBar->showMessage(QString("%1: %2 vs %3: %4")
        .arg(m_players[0]->name()).arg(m_players[0]->count())
        .arg(m_players[1]->name()).arg(m_players[1]->count()));
}

void DotsAndBoxes::showGameOver() {
    if (!m_gameOver) {
        Player* winner = qobject_cast<Player*>(QObject::sender());

        for (int r = 0; r < 7; r++) {
            for (int c = 0; c < 4; c++) {
                if (m_lines[r][c])
                    m_lines[r][c]->setEnabled(false);
            }
        }

        m_gameOver = true;
        QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador %1 venceu").arg(winner->name()));
    }
}

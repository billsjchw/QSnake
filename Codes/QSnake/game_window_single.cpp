#include "game_window_single.h"
#include <QKeyEvent>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QChar>
#include <QMessageBox>
#include "game_over_window_single.h"
#include "configuration.h"
#include "direction.h"

const QColor GameWindowSingle::COLOR_BROWN(76, 74, 12);

GameWindowSingle::GameWindowSingle() {
    ui = new Ui::GameWindowSingle;
    ui->setupUi(this);
    pauseWindow = new PauseWindow(this);
    if (conf.blockNum || conf.star == Configuration::ENABLE)
        pauseWindow->disableAutomatic();
    connect(pauseWindow, SIGNAL(finished(int)), this, SLOT(resume(int)));
    gameSingle = new GameSingle(this);
    connect(gameSingle, SIGNAL(update()), this, SLOT(repaint()));
    connect(gameSingle, SIGNAL(gameOver()), this, SLOT(handleGameOver()));
    repaint();
    gameSingle->start();
}

GameWindowSingle::~GameWindowSingle() {
    delete ui;
}

void GameWindowSingle::paintEvent(QPaintEvent *) {
    ui->score->setText(QString("%1").arg(gameSingle->snake->score, 3, 10, QChar('0')));
    QPainter painter(this);
    painter.translate(0, TITLE_HEIGHT);
    painter.scale(WIDTH / GameSingle::M, HEIGHT / GameSingle::N);
    for (int i = 0; i < GameSingle::M; ++i)
        for (int j = 0; j < GameSingle::N; ++j) {
            if (gameSingle->isBrick[i][j])
                painter.drawPixmap(i, j, 1, 1, IMG_BRICK);
            if (gameSingle->isBlock[i][j])
                painter.drawPixmap(i, j, 1, 1, IMG_BLOCK);
        }
    drawSnake(painter, gameSingle->snake, COLOR_BROWN);
    painter.drawPixmap(gameSingle->apple.x(), gameSingle->apple.y(), 1, 1, IMG_APPLE);
    if (gameSingle->starOn)
        painter.drawPixmap(gameSingle->star.x(), gameSingle->star.y(), 1, 1, IMG_STAR);
}

void GameWindowSingle::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
    case Qt::Key_Up: case Qt::Key_W: gameSingle->turnSnake(UP); break;
    case Qt::Key_Down: case Qt::Key_S: gameSingle->turnSnake(DOWN); break;
    case Qt::Key_Left: case Qt::Key_A: gameSingle->turnSnake(LEFT); break;
    case Qt::Key_Right: case Qt::Key_D: gameSingle->turnSnake(RIGHT); break;
    case Qt::Key_P: pause(); break;
    }
}

void GameWindowSingle::pause() {
    gameSingle->pause();
    pauseWindow->open();
}

void GameWindowSingle::resume(int automaticFlag) {
    if (automaticFlag)
        gameSingle->startAutomatic();
    else
        gameSingle->closeAutomatic();
    gameSingle->resume();
}

void GameWindowSingle::handleGameOver() {
    GameOverWindowSingle * gameOverWindowSingle = new GameOverWindowSingle(gameSingle->snake->score);
    gameOverWindowSingle->show();
    close();
}

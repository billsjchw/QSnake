#include "game_window_double.h"
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QChar>
#include "game_over_window_double.h"
#include "direction.h"

const QColor GameWindowDouble::COLOR_RED(211, 11, 55);
const QColor GameWindowDouble::COLOR_BLUE(0, 69, 155);

GameWindowDouble::GameWindowDouble() {
    ui = new Ui::GameWindowDouble;
    ui->setupUi(this);
    pauseWindow = new PauseWindow(this);
    pauseWindow->disableAutomatic();
    connect(pauseWindow, SIGNAL(finished(int)), this, SLOT(resume()));
    gameDouble = new GameDouble(this);
    connect(gameDouble, SIGNAL(update()), this, SLOT(repaint()));
    connect(gameDouble, SIGNAL(gameOver()), this, SLOT(handleGameOver()));
    repaint();
    gameDouble->start();
}

GameWindowDouble::~GameWindowDouble() {
    delete ui;
}

void GameWindowDouble::paintEvent(QPaintEvent *) {
    ui->scoreRed->setText(QString("%1").arg(gameDouble->red->score, 3, 10, QChar('0')));
    ui->scoreBlue->setText(QString("%1").arg(gameDouble->blue->score, 3, 10, QChar('0')));
    QPainter painter(this);
    painter.translate(0, TITLE_HEIGHT);
    painter.scale(WIDTH / GameDouble::M, HEIGHT / GameDouble::N);
    for (int i = 0; i < GameDouble::M; ++i)
        for (int j = 0; j < GameDouble::N; ++j) {
            if (gameDouble->isBrick[i][j])
                painter.drawPixmap(i, j, 1, 1, IMG_BRICK);
            if (gameDouble->isBlock[i][j])
                painter.drawPixmap(i, j, 1, 1, IMG_BLOCK);
        }
    drawSnake(painter, gameDouble->red, COLOR_RED);
    drawSnake(painter, gameDouble->blue, COLOR_BLUE);
    painter.drawPixmap(gameDouble->apple.x(), gameDouble->apple.y(), 1, 1, IMG_APPLE);
    if (gameDouble->starOn)
        painter.drawPixmap(gameDouble->star.x(), gameDouble->star.y(), 1, 1, IMG_STAR);
}

void GameWindowDouble::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
    case Qt::Key_W: gameDouble->turnSnakeRed(UP); break;
    case Qt::Key_S: gameDouble->turnSnakeRed(DOWN); break;
    case Qt::Key_A: gameDouble->turnSnakeRed(LEFT); break;
    case Qt::Key_D: gameDouble->turnSnakeRed(RIGHT); break;
    case Qt::Key_Up: gameDouble->turnSnakeBlue(UP); break;
    case Qt::Key_Down: gameDouble->turnSnakeBlue(DOWN); break;
    case Qt::Key_Left: gameDouble->turnSnakeBlue(LEFT); break;
    case Qt::Key_Right: gameDouble->turnSnakeBlue(RIGHT); break;
    case Qt::Key_P: pause(); break;
    }
}

void GameWindowDouble::pause() {
    gameDouble->pause();
    pauseWindow->open();
}

void GameWindowDouble::resume() {
    gameDouble->resume();
}

void GameWindowDouble::handleGameOver() {
    GameOverWindowDouble * gameOverWindowDouble = new GameOverWindowDouble(gameDouble->red->score, gameDouble->blue->score);
    gameOverWindowDouble->show();
    close();
}

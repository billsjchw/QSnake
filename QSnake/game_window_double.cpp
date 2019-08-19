#include "game_window_double.h"
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QChar>
#include "game_over_window_double.h"

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
    for (QPoint p: gameDouble->red->body)
        painter.drawPixmap(p.x(), p.y(), 1, 1, IMG_SNAKE_RED);
    for (QPoint p: gameDouble->blue->body)
        painter.drawPixmap(p.x(), p.y(), 1, 1, IMG_SNAKE_BLUE);
    painter.drawPixmap(gameDouble->apple.x(), gameDouble->apple.y(), 1, 1, IMG_APPLE);
    if (gameDouble->starOn)
        painter.drawPixmap(gameDouble->star.x(), gameDouble->star.y(), 1, 1, IMG_STAR);
}

void GameWindowDouble::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
    case Qt::Key_W: gameDouble->turnSnakeRed(Snake::UP); break;
    case Qt::Key_S: gameDouble->turnSnakeRed(Snake::DOWN); break;
    case Qt::Key_A: gameDouble->turnSnakeRed(Snake::LEFT); break;
    case Qt::Key_D: gameDouble->turnSnakeRed(Snake::RIGHT); break;
    case Qt::Key_Up: gameDouble->turnSnakeBlue(Snake::UP); break;
    case Qt::Key_Down: gameDouble->turnSnakeBlue(Snake::DOWN); break;
    case Qt::Key_Left: gameDouble->turnSnakeBlue(Snake::LEFT); break;
    case Qt::Key_Right: gameDouble->turnSnakeBlue(Snake::RIGHT); break;
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

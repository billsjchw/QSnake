#include "game_double.h"
#include <QtGlobal>
#include "configuration.h"
#include <QString>
#include <QChar>
#include <QFile>
#include <QTextStream>
#include "direction.h"

const char * GameDouble::MAP_FILENAME = ":/map/%1.double.map";

GameDouble::GameDouble(QObject * parent): Game(parent, M, N) {
    if (conf.bricks == Configuration::DISABLE) {
        red = new Snake(this, INIT_SNAKE_LEN, QPoint(M / 3, N / 2), RIGHT, M, N);
        blue = new Snake(this, INIT_SNAKE_LEN, QPoint(2 * M / 3, N / 2), LEFT, M, N);
    } else
        loadMap();
    connect(red->timer, SIGNAL(timeout()), this, SLOT(updateSnakeRed()));
    connect(blue->timer, SIGNAL(timeout()), this, SLOT(updateSnakeBlue()));
    connect(red, SIGNAL(speedUpFinished()), this, SLOT(emitUpdate()));
    connect(blue, SIGNAL(speedUpFinished()), this, SLOT(emitUpdate()));
    newApple();
    resetBlocks();
}

void GameDouble::start() {
    Game::start();
    red->timer->start();
    blue->timer->start();
}

void GameDouble::pause() {
    Game::pause();
    red->pauseSpeedUp();
    blue->pauseSpeedUp();
}

void GameDouble::resume() {
    Game::resume();
    red->resumeSpeedUp();
    blue->resumeSpeedUp();
}

int GameDouble::snakeLength() {
    return red->body.length() + blue->body.length();
}

void GameDouble::updateIsSnake() {
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            isSnake[i][j] = false;
    for (QPoint p: red->body)
        isSnake[p.x()][p.y()] = true;
    for (QPoint p: blue->body)
        isSnake[p.x()][p.y()] = true;
}

void GameDouble::updateSnakeRed() {
    updateSnake(red);
}

void GameDouble::updateSnakeBlue() {
    updateSnake(blue);
}

void GameDouble::updateSnake(Snake * snake) {
    QPoint p = snake->next();
    if (p == apple) {
        snake->eat();
        snake->updateScore(APPLE_SCORE);
        snake->updateInterval();
        if (snakeLength() + brickNum + conf.blockNum == M * N)
            handleGameOver();
        else {
            newApple();
            emit update();
        }
    } else if (starOn && p == star) {
        snake->eat();
        starOn = false;
        snake->updateScore(STAR_SCORE);
        snake->startSpeedUp();
        snake->updateInterval();
        emit update();
    } else if (!inside(p) || isBrick[p.x()][p.y()] || isBlock[p.x()][p.y()])
        handleGameOver();
    else if (twoSnakeCrash(p, reverse(snake)))
        handleGameOver();
    else if (!snake->move())
        handleGameOver();
    else
        emit update();
}

bool GameDouble::twoSnakeCrash(QPoint head, Snake * snake) {
    for (QPoint p: snake->body)
        if (head == p)
            return true;
    return false;
}

Snake * GameDouble::reverse(Snake * snake) {
    return snake == red ? blue : red;
}

void GameDouble::loadMap() {
    int k = qrand() % MAP_NUM;
    QString filename = QString(MAP_FILENAME).arg(k, 2, 10, QChar('0'));
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QTextStream fin(&file);
    for (Snake ** snake: {&red, &blue}) {
        int len, headX, headY;
        char wasd, space;
        fin >> len >> space >> wasd >> headX >> headY;
        QPoint head(headX, headY);
        QPoint dir;
        switch (wasd) {
        case 'w': dir = UP; break;
        case 'a': dir = LEFT; break;
        case 's': dir = DOWN; break;
        case 'd': dir = RIGHT; break;
        }
        *snake = new Snake(this, len, head, dir, M, N);
    }
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            isBrick[i][j] = false;
    fin >> brickNum;
    for (int i = 0; i < brickNum; ++i) {
        int x, y;
        fin >> x >> y;
        isBrick[x][y] = true;
    }
}

void GameDouble::turnSnakeRed(QPoint dir) {
    turnSnake(red, dir);
}

void GameDouble::turnSnakeBlue(QPoint dir) {
    turnSnake(blue, dir);
}

void GameDouble::turnSnake(Snake * snake, QPoint dir) {
    if (pauseFlag)
        return;
    snake->turn(dir);
}

void GameDouble::handleGameOver() {
    for (Snake * snake: {red, blue}) {
        snake->timer->stop();
        snake->speedUpTimer->stop();
    }
    Game::handleGameOver();
}

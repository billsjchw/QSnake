#include "game_single.h"
#include <QtGlobal>
#include <QDateTime>
#include <QVector>
#include <QtMath>
#include "configuration.h"
#include <QString>
#include <QChar>
#include <QFile>
#include <QTextStream>
#include "direction.h"

const char * GameSingle::MAP_FILENAME = ":/map/%1.single.map";

GameSingle::GameSingle(QObject * parent): Game(parent, M, N), automaticFlag(false) {
    if (conf.bricks == Configuration::DISABLE)
        snake = new Snake(this, INIT_SNAKE_LEN, QPoint(M / 2, N / 2), RIGHT, M, N);
    else
        loadMap();
    connect(snake->timer, SIGNAL(timeout()), this, SLOT(updateSnake()));
    connect(snake, SIGNAL(speedUpFinished()), this, SLOT(emitUpdate()));
    newApple();
    resetBlocks();
}

void GameSingle::start() {
    Game::start();
    snake->timer->start();
}

void GameSingle::pause() {
    Game::pause();
    snake->pauseSpeedUp();
}

void GameSingle::resume() {
    Game::resume();
    snake->resumeSpeedUp();
}

void GameSingle::turnSnake(QPoint dir) {
    if (pauseFlag || automaticFlag)
        return;
    snake->turn(dir);
}

void GameSingle::updateSnake() {
    if (pauseFlag)
        return;
    if (automaticFlag) {
        if (strategy.empty())
            makeStrategy();
        snake->turn(strategy.front());
        strategy.pop_front();
    }
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
    else if (!snake->move())
        handleGameOver();
    else
        emit update();
}

void GameSingle::handleGameOver() {
    snake->timer->stop();
    snake->speedUpTimer->stop();
    Game::handleGameOver();
}

void GameSingle::startAutomatic() {
    automaticFlag = true;
    snake->clearOperations();
}

void GameSingle::closeAutomatic() {
    automaticFlag = false;
}

void GameSingle::makeStrategy() {
    QList<QPoint> path = shortestPathToApple();
    if (!path.empty() && isSafePathToApple(path))
        for (int i = 0; i < path.length() - 1; ++i)
            strategy.push_back(path[i + 1] - path[i]);
    else if (hasPathToTail(snake->body))
        strategy.push_back(directionToTail());
    else
        strategy.push_back(randomDirection());
}

QPoint GameSingle::randomDirection() {
    QVector<QPoint> validDirection;
    for (QPoint dir: {LEFT, RIGHT, UP, DOWN})
        if (dir != -snake->dir && !snake->crash(dir)) {
            QPoint p = snake->next(dir);
            if (inside(p) && !isBrick[p.x()][p.y()])
                validDirection.push_back(dir);
        }
    if (validDirection.empty())
        return LEFT;
    int k = rand() % validDirection.length();
    return validDirection[k];
}

QList<QPoint> GameSingle::shortestPathToApple() {
    QVector<QVector<bool>> obstacle = isBrick;
    for (int i = 1; i < snake->body.length(); ++i)
        obstacle[snake->body[i].x()][snake->body[i].y()] = true;
    QPoint head = snake->body[0];
    return bfs(head, apple, obstacle);
}

bool GameSingle::isSafePathToApple(const QList<QPoint> & path) {
    return hasPathToTail(snake->eat(path));
}

bool GameSingle::hasPathToTail(const QList<QPoint> & body) {
    QVector<QVector<bool>> obstacle = isBrick;
    for (int i = 1; i < body.length() - 1; ++i)
        obstacle[body[i].x()][body[i].y()] = true;
    QPoint head = body.front();
    QPoint tail = body.back();
    return bfs(head, tail, obstacle).length();
}

QPoint GameSingle::directionToTail() {
    int m = 0;
    QPoint ret;
    for (QPoint dir: {LEFT, RIGHT, UP, DOWN})
        if (dir != -snake->dir) {
            QPoint p = snake->next(dir);
            if (inside(p) && !isBrick[p.x()][p.y()]) {
                QList<QPoint> body;
                if (snake->next(dir) == apple)
                    body = snake->eat(dir);
                else
                    body = snake->move(dir);
                if (body.length() && hasPathToTail(body) && manhattan(body.front(), body.back()) > m) {
                    m = manhattan(body.front(), body.back());
                    ret = dir;
                }
            }
        }
    return ret;
}

int GameSingle::snakeLength() {
    return snake->body.length();
}

void GameSingle::updateIsSnake() {
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            isSnake[i][j] = false;
    for (QPoint p: snake->body)
        isSnake[p.x()][p.y()] = true;
}

QList<QPoint> GameSingle::bfs(QPoint u, QPoint v, const QVector<QVector<bool>> & obstacle) {
    QVector<QVector<bool>> vis(M);
    for (QVector<bool> & yaxis: vis)
        yaxis.resize(N);
    QVector<QVector<QPoint>> prev(M);
    for (QVector<QPoint> & yaxis: prev)
        yaxis.resize(N);
    QQueue<QPoint> q;
    vis[u.x()][u.y()] = true;
    prev[u.x()][u.y()] = u;
    q.enqueue(u);
    while (!vis[v.x()][v.y()] && !q.empty()) {
        for (QPoint dir: {LEFT, RIGHT, UP, DOWN}) {
            QPoint p = q.front() + dir;
            if (conf.through == Configuration::ENABLE) {
                p.setX((p.x() + M) % M);
                p.setY((p.y() + N) % N);
            }
            if (inside(p) && !obstacle[p.x()][p.y()] && !vis[p.x()][p.y()]) {
                vis[p.x()][p.y()] = true;
                prev[p.x()][p.y()] = q.front();
                q.push_back(p);
            }
        }
        q.dequeue();
    }
    if (!vis[v.x()][v.y()])
        return QList<QPoint>();
    QList<QPoint> path;
    for (QPoint p = v; p != u; p = prev[p.x()][p.y()])
        path.push_front(p);
    path.push_front(u);
    return path;
}

int GameSingle::manhattan(QPoint u, QPoint v) {
    int dx = qAbs(u.x() - v.x());
    int dy = qAbs(u.y() - v.y());
    if (conf.through == Configuration::ENABLE)
        return qMin(dx, M - dx) + qMin(dy, N - dy);
    return dx + dy;
}

void GameSingle::loadMap() {
    int k = qrand() % MAP_NUM;
    QString filename = QString(MAP_FILENAME).arg(k, 2, 10, QChar('0'));
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QTextStream fin(&file);
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
    snake = new Snake(this, len, head, dir, M, N);
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

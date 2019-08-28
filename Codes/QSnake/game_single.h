#ifndef GAME_SINGLE_H
#define GAME_SINGLE_H

#include "game.h"
#include <QPoint>
#include <QQueue>
#include <QList>
#include <QVector>
#include "snake.h"

class GameSingle: public Game {
    Q_OBJECT
public:
    static const int M = 20;
    static const int N = 20;
private:
    static const int INIT_SNAKE_LEN = 3;
    static const int MAP_NUM = 2;
    static const int SPEED_UP_INTERVAL = 10000;
    static const char * MAP_FILENAME;
public:
    Snake * snake;
private:
    bool automaticFlag;
    QQueue<QPoint> strategy;
public:
    GameSingle(QObject * parent);
    void start();
    void pause();
    void resume();
    void turnSnake(QPoint dir);
    void startAutomatic();
    void closeAutomatic();
private:
    void handleGameOver();
    void makeStrategy();
    QPoint randomDirection();
    QList<QPoint> shortestPathToApple();
    bool isSafePathToApple(const QList<QPoint> & path);
    bool hasPathToTail(const QList<QPoint> & body);
    QPoint directionToTail();
    int snakeLength();
    void updateIsSnake();
    QList<QPoint> bfs(QPoint u, QPoint v, const QVector<QVector<bool>> & obstacle);
    int manhattan(QPoint u, QPoint v);
    void loadMap();
    void startSpeedUp();
private slots:
    void updateSnake();
};

#endif // GAME_SINGLE_H

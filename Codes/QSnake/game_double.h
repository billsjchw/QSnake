#ifndef GAME_DOUBLE_H
#define GAME_DOUBLE_H

#include "game.h"
#include "snake.h"
#include <QPoint>

class GameDouble: public Game {
    Q_OBJECT
public:
    static const int M = 30;
    static const int N = 20;
private:
    static const int INIT_SNAKE_LEN = 3;
    static const int MAP_NUM = 1;
    static const char * MAP_FILENAME;
public:
    Snake * red, * blue;
public:
    GameDouble(QObject * parent);
    void start();
    void pause();
    void resume();
    void turnSnakeRed(QPoint dir);
    void turnSnakeBlue(QPoint dir);
private:
    int snakeLength();
    void updateIsSnake();
    void updateSnake(Snake * snake);
    void loadMap();
    void turnSnake(Snake * snake, QPoint dir);
    bool twoSnakeCrash(QPoint head, Snake * snake);
    Snake * reverse(Snake * snake);
    void handleGameOver();
private slots:
    void updateSnakeRed();
    void updateSnakeBlue();
};

#endif // GAME_DOUBLE_H

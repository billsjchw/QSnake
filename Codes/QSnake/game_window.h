#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QColor>
#include "snake.h"

class GameWindow: public QWidget {
protected:
    const QPixmap IMG_APPLE;
    const QPixmap IMG_BRICK;
    const QPixmap IMG_BLOCK;
    const QPixmap IMG_STAR;
private:
    const int MASK_WIDTH = 151;
    const int MASK_HEIGHT = 151;
    const QBitmap IMG_BODY;
    const QBitmap IMG_HEAD_TAIL_LEFT;
    const QBitmap IMG_HEAD_TAIL_RIGHT;
    const QBitmap IMG_HEAD_TAIL_UP;
    const QBitmap IMG_HEAD_TAIL_DOWN;
public:
    GameWindow();
protected:
    void drawSnake(QPainter & painter, const Snake * snake, QColor color);
};

#endif // GAME_WINDOW_H

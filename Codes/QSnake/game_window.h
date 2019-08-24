#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QWidget>

class GameWindow: public QWidget {
protected:
    const QPixmap IMG_SNAKE;
    const QPixmap IMG_APPLE;
    const QPixmap IMG_BRICK;
    const QPixmap IMG_BLOCK;
    const QPixmap IMG_STAR;
    const QPixmap IMG_SNAKE_RED;
    const QPixmap IMG_SNAKE_BLUE;
public:
    GameWindow();
};

#endif // GAME_WINDOW_H

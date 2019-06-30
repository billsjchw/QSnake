#include "qsnake.h"

const QPoint QSnake::OFFSET[4] = {
    QPoint(-1, 0),
    QPoint(0, -1),
    QPoint(0, 1),
    QPoint(1, 0)
};
const QPointF QSnake::POLY_SNAKE[QSnake::POLY_SNAKE_N] = {
    QPointF(0.3, 0.1), QPointF(0.7, 0.1),
    QPointF(0.9, 0.3), QPointF(0.9, 0.7),
    QPointF(0.7, 0.9), QPointF(0.3, 0.9),
    QPointF(0.1, 0.7), QPointF(0.1, 0.3)
};
const QPointF QSnake::POLY_FOOD[QSnake::POLY_FOOD_N] = {
    QPointF(0.5, 0.1),
    QPointF(0.1, 0.9),
    QPointF(0.9, 0.9)
};
const QColor QSnake::COLOUR_BACKGROUND(157, 164, 95);
const QColor QSnake::COLOUR_ITEM(76, 74, 12);
const QColor QSnake::COLOUR_ALPHA_BLACK(0, 0, 0, 90);
const QString QSnake::ICON_START(":/img/start.png");
const QImage QSnake::ICON_CUP_WHITE(":/img/cup_white.png");
const QImage QSnake::ICON_CUP_ITEM(":/img/cup_item.png");
const QString QSnake::ICON_RESTART(":/img/restart.png");
const QString QSnake::ICON_SETTING(":/img/setting.png");
const QImage QSnake::IMG_GAME_OVER(":/img/game_over.png");
const QFont QSnake::FONT_BRITANIC("Britannic Bold", 18);
const QFont QSnake::FONT_BRITANIC_LARGE("Britannic Bold", 30);

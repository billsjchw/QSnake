#include "game_window.h"

GameWindow::GameWindow():
    IMG_SNAKE(":/img/snake.png", "PNG"),
    IMG_APPLE(":/img/apple.png", "PNG"),
    IMG_BRICK(":/img/brick.png", "PNG"),
    IMG_BLOCK(":/img/block.png", "PNG"),
    IMG_STAR(":/img/star.png", "PNG"),
    IMG_SNAKE_RED(":/img/snake_red.png", "PNG"),
    IMG_SNAKE_BLUE(":/img/snake_blue.png", "PNG") {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);
}

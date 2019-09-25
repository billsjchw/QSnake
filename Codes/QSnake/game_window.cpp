#include "game_window.h"
#include <QPoint>
#include "direction.h"

GameWindow::GameWindow():
    IMG_APPLE(":/img/apple.png", "PNG"),
    IMG_BRICK(":/img/brick.png", "PNG"),
    IMG_BLOCK(":/img/block.png", "PNG"),
    IMG_STAR(":/img/star.png", "PNG"),
    IMG_BODY(":/img/body.png", "PNG"),
    IMG_HEAD_TAIL_LEFT(":/img/head_tail_left.png", "PNG"),
    IMG_HEAD_TAIL_RIGHT(":/img/head_tail_right.png", "PNG"),
    IMG_HEAD_TAIL_UP(":/img/head_tail_up.png", "PNG"),
    IMG_HEAD_TAIL_DOWN(":/img/head_tail_down.png", "PNG") {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);
}

void GameWindow::drawSnake(QPainter & painter, const Snake * snake, QColor color) {
    QPixmap imgBody(MASK_WIDTH, MASK_HEIGHT);
    QPixmap imgHeadTailLeft(MASK_WIDTH, MASK_HEIGHT);
    QPixmap imgHeadTailRight(MASK_WIDTH, MASK_HEIGHT);
    QPixmap imgHeadTailUp(MASK_WIDTH, MASK_HEIGHT);
    QPixmap imgHeadTailDown(MASK_WIDTH, MASK_HEIGHT);
    for (QPixmap * pixmap: {&imgBody, &imgHeadTailLeft, &imgHeadTailRight, &imgHeadTailUp, &imgHeadTailDown})
        pixmap->fill(color);
    imgBody.setMask(IMG_BODY);
    imgHeadTailLeft.setMask(IMG_HEAD_TAIL_LEFT);
    imgHeadTailRight.setMask(IMG_HEAD_TAIL_RIGHT);
    imgHeadTailUp.setMask(IMG_HEAD_TAIL_UP);
    imgHeadTailDown.setMask(IMG_HEAD_TAIL_DOWN);
    int len = snake->body.length();
    QPoint head = snake->body[0];
    QPoint headDir = snake->dir;
    if (headDir == LEFT)
        painter.drawPixmap(head.x(), head.y(), 1, 1, imgHeadTailLeft);
    else if (headDir == RIGHT)
        painter.drawPixmap(head.x(), head.y(), 1, 1, imgHeadTailRight);
    else if (headDir == UP)
        painter.drawPixmap(head.x(), head.y(), 1, 1, imgHeadTailUp);
    else
        painter.drawPixmap(head.x(), head.y(), 1, 1, imgHeadTailDown);
    for (int i = 1; i < len - 1; ++i) {
        QPoint p = snake->body[i];
        painter.drawPixmap(p.x(), p.y(), 1, 1, imgBody);
    }
    QPoint tail = snake->body[len - 1];
    QPoint tailDir = snake->tailDirection();
    if (tailDir == LEFT)
        painter.drawPixmap(tail.x(), tail.y(), 1, 1, imgHeadTailLeft);
    else if (tailDir == RIGHT)
        painter.drawPixmap(tail.x(), tail.y(), 1, 1, imgHeadTailRight);
    else if (tailDir == UP)
        painter.drawPixmap(tail.x(), tail.y(), 1, 1, imgHeadTailUp);
    else
        painter.drawPixmap(tail.x(), tail.y(), 1, 1, imgHeadTailDown);
}

#include "snake.h"
#include "configuration.h"

Snake::Snake(QObject * parent, int len, QPoint head, QPoint dir, int m, int n):
    dir(dir), interval(MAX_INTERVAL), score(0), speedUp(false), m(m), n(n) {
    setParent(parent);
    timer = new QTimer(this);
    timer->setInterval(MAX_INTERVAL);
    speedUpTimer = new QTimer(this);
    speedUpTimer->setSingleShot(true);
    connect(speedUpTimer, SIGNAL(timeout()), this, SLOT(closeSpeedUp()));
    body.push_back(head);
    for (int i = 1; i < len; ++i)
        body.push_back(body[i - 1] - dir);
}

void Snake::turn(QPoint dir) {
    QPoint prev = ops.empty() ? this->dir : ops.back();
    if (prev != dir && prev != -dir)
        ops.enqueue(dir);
}

QPoint Snake::next() {
    if (!ops.empty())
        return go(body.front(), ops.front());
    return go(body.front(), dir);
}

QPoint Snake::next(QPoint dir) {
    return go(body.front(), dir);
}

bool Snake::move() {
    if (!ops.empty())
        dir = ops.dequeue();
    if (crash(dir))
        return false;
    body.push_front(go(body[0], dir));
    body.pop_back();
    return true;
}

QList<QPoint> Snake::move(QPoint dir) {
    if (crash(dir))
        return QList<QPoint>();
    QList<QPoint> ret = body;
    ret.push_front(go(ret[0], dir));
    ret.pop_back();
    return ret;
}

void Snake::eat() {
    if (!ops.empty())
        dir = ops.dequeue();
    QPoint newHead = go(body.front(), dir);
    body.push_front(newHead);
}

QList<QPoint> Snake::eat(const QList<QPoint> & path) {
    QList<QPoint> ret = body;
    for (int i = 1; i < path.length() - 1; ++i) {
        ret.pop_back();
        ret.push_front(path[i]);
    }
    ret.push_front(path.back());
    return ret;
}

QList<QPoint> Snake::eat(QPoint dir) {
    QList<QPoint> ret = body;
    ret.push_front(go(ret[0], dir));
    return ret;
}

void Snake::clearOperations() {
    ops.clear();
}

bool Snake::crash(QPoint dir) {
    for (int i = 0; i < body.length() - 1; ++i)
        if (go(body[0], dir) == body[i])
            return true;
    return false;
}

void Snake::startSpeedUp() {
    speedUp = true;
    timer->setInterval(SPEED_UP_INTERVAL);
    speedUpTimer->setInterval(SPEED_UP_DURATION);
    speedUpTimer->start();
}

void Snake::closeSpeedUp() {
    speedUp = false;
    timer->setInterval(interval);
    emit speedUpFinished();
}

void Snake::pauseSpeedUp() {
    if (speedUp) {
        speedUpTimeLeft = speedUpTimer->remainingTime();
        speedUpTimer->stop();
    }
}

void Snake::resumeSpeedUp() {
    if (speedUp) {
        speedUpTimer->setInterval(speedUpTimeLeft);
        speedUpTimer->start();
    }
}

void Snake::updateScore(int inc) {
    score += inc;
}

QPoint Snake::tailDirection() const {
    int len = body.length();
    QPoint ret = body[len - 1] - body[len - 2];
    if (ret.x() == m - 1)
        ret.setX(-1);
    else if (ret.x() == 1 - m)
        ret.setX(1);
    else if (ret.y() == n - 1)
        ret.setY(-1);
    else if (ret.y() == 1 - n)
        ret.setY(1);
    return ret;
}

void Snake::updateInterval() {
    if (interval > MIN_INTERVAL)
        interval -= INTERVAL_OFFSET;
    if (!speedUp)
        timer->setInterval(interval);
}

QPoint Snake::go(QPoint u, QPoint dir) {
    QPoint v = u + dir;
    if (conf.through == Configuration::ENABLE) {
        v.setX((v.x() + m) % m);
        v.setY((v.y() + n) % n);
    }
    return v;
}

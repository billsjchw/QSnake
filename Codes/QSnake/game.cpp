#include "game.h"
#include "configuration.h"
#include <QDateTime>
Game::Game(QObject * parent, int m, int n):
    m(m), n(n), isBrick(m), isBlock(m), starOn(false), isSnake(m),
    brickNum(0), pauseFlag(false) {
    setParent(parent);
    for (QVector<bool> & yaxis: isSnake)
        yaxis.resize(n);
    for (QVector<bool> & yaxis: isBrick)
        yaxis.resize(n);
    for (QVector<bool> & yaxis: isBlock)
        yaxis.resize(n);
    qsrand(QDateTime::currentDateTime().toTime_t());
    blockTimer = new QTimer(this);
    blockTimer->setSingleShot(true);
    connect(blockTimer, SIGNAL(timeout()), this, SLOT(resetBlocks()));
    starOffTimer = new QTimer(this);
    starOffTimer->setSingleShot(true);
    connect(starOffTimer, SIGNAL(timeout()), this, SLOT(starAppear()));
    starOnTimer = new QTimer(this);
    starOnTimer->setSingleShot(true);
    connect(starOnTimer, SIGNAL(timeout()), this, SLOT(starDisappear()));
}

void Game::start() {
    if (conf.blockNum) {
        blockTimer->setInterval(BLOCK_INTERVAL);
        blockTimer->start();
    }
    if (conf.star) {
        starOffTimer->setInterval(STAR_OFF_INTERVAL);
        starOffTimer->start();
    }
}

void Game::pause() {
    pauseFlag = true;
    starOnTimeLeft = starOnTimer->isActive() ? starOnTimer->remainingTime() : NOT_ACTIVE;
    starOffTimeLeft = starOffTimer->isActive() ? starOffTimer->remainingTime() : NOT_ACTIVE;
    blockTimeLeft = blockTimer->remainingTime();
    starOnTimer->stop();
    starOffTimer->stop();
    blockTimer->stop();
}

void Game::resume() {
    pauseFlag = false;
    if (starOnTimeLeft != NOT_ACTIVE) {
        starOnTimer->setInterval(starOnTimeLeft);
        starOnTimer->start();
    }
    if (starOffTimeLeft != NOT_ACTIVE) {
        starOffTimer->setInterval(starOffTimeLeft);
        starOffTimer->start();
    }
    blockTimer->setInterval(blockTimeLeft);
    blockTimer->start();
}

bool Game::inside(QPoint p) {
    return p.x() >= 0 && p.x() < m && p.y() >= 0 && p.y() < n;
}

void Game::newApple() {
    updateIsSnake();
    int spaceLeft = m * n - snakeLength() - brickNum - conf.blockNum - starOn;
    if (!spaceLeft) {
        starOn = false;
        spaceLeft = 1;
    }
    int k = qrand() % spaceLeft + 1;
    QPoint p = QPoint(0, -1);
    for (int i = 0; i < k; ++i)
        do {
            p += QPoint(0, 1);
            if (p.y() == n) {
                p += QPoint(1, 0);
                p.setY(0);
            }
        } while (isSnake[p.x()][p.y()] || isBrick[p.x()][p.y()] || isBlock[p.x()][p.y()] || (starOn && star == p));
    apple = p;
}


void Game::handleGameOver() {
    blockTimer->stop();
    starOnTimer->stop();
    starOffTimer->stop();
    emit gameOver();
}

void Game::resetBlocks() {
    updateIsSnake();
    int spaceLeft = m * n - snakeLength() - brickNum - 1 - starOn;
    QList<int> candidate;
    for (int i = 1; i <= spaceLeft; ++i)
        candidate.push_back(i);
    QList<int> chosen;
    for (int i = 0; i < conf.blockNum; ++i) {
        int k = qrand() % candidate.length();
        chosen.push_back(candidate[k]);
        candidate.removeAt(k);
    }
    int cnt = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            isBlock[i][j] = false;
            if (!isSnake[i][j] && !isBrick[i][j] && apple != QPoint(i, j) && (!starOn || star != QPoint(i, j))) {
                ++cnt;
                if (chosen.contains(cnt)) {
                    isBlock[i][j] = true;
                    chosen.removeAll(cnt);
                }
            }
        }
    emit update();
    blockTimer->setInterval(BLOCK_INTERVAL);
    blockTimer->start();
}

void Game::starAppear() {
    updateIsSnake();
    int spaceLeft = m * n - snakeLength() - brickNum - conf.blockNum - 1;
    if (!spaceLeft)
        return;
    int k = qrand() % spaceLeft + 1;
    QPoint p = QPoint(0, -1);
    for (int i = 0; i < k; ++i)
        do {
            p += QPoint(0, 1);
            if (p.y() == n) {
                p += QPoint(1, 0);
                p.setY(0);
            }
        } while (isSnake[p.x()][p.y()] || isBrick[p.x()][p.y()] || isBlock[p.x()][p.y()] || apple == p);
    star = p;
    starOn = true;
    starOnTimer->setInterval(STAR_ON_INTERVAL);
    starOnTimer->start();
    emit update();
}

void Game::starDisappear() {
    starOn = false;
    starOffTimer->setInterval(STAR_OFF_INTERVAL);
    starOffTimer->start();
    emit update();
}

void Game::emitUpdate() {
    emit update();
}

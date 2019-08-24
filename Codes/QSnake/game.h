#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QTimer>

class Game: public QObject {
    Q_OBJECT
protected:
    static const int BLOCK_INTERVAL = 8000;
    static const int STAR_ON_INTERVAL = 15000;
    static const int STAR_OFF_INTERVAL = 8000;
    static const int APPLE_SCORE = 1;
    static const int STAR_SCORE = 5;
    static const int NOT_ACTIVE = -1;
private:
    int m, n;
public:
    QPoint apple, star;
    QVector<QVector<bool>> isBrick;
    QVector<QVector<bool>> isBlock;
    bool starOn;
protected:
    QVector<QVector<bool>> isSnake;
    int brickNum;
    bool pauseFlag;
    QTimer * blockTimer;
    int blockTimeLeft;
    QTimer * starOnTimer;
    int starOnTimeLeft;
    QTimer * starOffTimer;
    int starOffTimeLeft;
public:
    Game(QObject * parent, int m, int n);
    void start();
    void pause();
    void resume();
protected:
    bool inside(QPoint p);
    virtual int snakeLength() = 0;
    virtual void updateIsSnake() = 0;
    virtual void newApple();
    void handleGameOver();
protected slots:
    virtual void resetBlocks();
    virtual void starAppear();
    void starDisappear();
    void emitUpdate();
signals:
    void update();
    void gameOver();
};

#endif // GAME_H

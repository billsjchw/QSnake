#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <QList>
#include <QPoint>
#include <QQueue>
#include <QTimer>

class Snake: public QObject {
    Q_OBJECT
public:
    static const int MAX_INTERVAL = 200;
    static const int MIN_INTERVAL = 100;
    static const int INTERVAL_OFFSET = 5;
    static const int SPEED_UP_INTERVAL = 70;
    static const int SPEED_UP_DURATION = 10000;
public:
    QList<QPoint> body;
    QPoint dir;
    int interval;
    int score;
    QTimer * timer;
    QTimer * speedUpTimer;
    bool speedUp;
private:
    QQueue<QPoint> ops;
    int speedUpTimeLeft;
    int m, n;
public:
    Snake(QObject * parent, int len, QPoint head, QPoint dir, int m, int n);
    void turn(QPoint dir);
    QPoint next();
    QPoint next(QPoint dir);
    bool move();
    QList<QPoint> move(QPoint dir);
    void eat();
    QList<QPoint> eat(const QList<QPoint> & path);
    QList<QPoint> eat(QPoint dir);
    void clearOperations();
    bool crash(QPoint dir);
    void updateInterval();
    void startSpeedUp();
    void pauseSpeedUp();
    void resumeSpeedUp();
    void updateScore(int inc);
    QPoint tailDirection() const;
private:
    QPoint go(QPoint u, QPoint dir);
private slots:
    void closeSpeedUp();
signals:
    void speedUpFinished();
};

#endif // SNAKE_H

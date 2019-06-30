#ifndef QSNAKE_H
#define QSNAKE_H

#include <Qt>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QtEvents>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QString>
#include <QChar>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QPen>
#include <deque>
#include <stack>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

class QSnake: public QWidget {
Q_OBJECT
private:
    enum State {WELCOME, PLAYING, GAMEOVER, SETTING, PAUSE};
    enum Direction {LEFT, UP, DOWN, RIGHT};
    struct Snake {
        static const int INIT_LEN = 3;
        int len;
        Direction dir;
        std::deque<QPoint> shape;
        Snake() {}
        Snake(int m, int n): len(INIT_LEN) {
            QPoint head(m / 2, n / 2);
            dir = Direction(rand() % 4);
            for (int i = INIT_LEN - 1; i >= 0; --i)
                shape.push_back(head + OFFSET[3 - dir] * i);
        }
    };
private:
    static const int WIDTH_MAIN = 800;
    static const int HEIGHT_MAIN = 900;
    static const int HEIGHT_PG = 800;
    static const int ITEM_SIZE = 40;
    static const int M = WIDTH_MAIN / ITEM_SIZE;
    static const int N = HEIGHT_PG / ITEM_SIZE;
    static const QPoint OFFSET[4];
    static const int POLY_SNAKE_N = 8;
    static const QPointF POLY_SNAKE[POLY_SNAKE_N];
    static const int POLY_FOOD_N = 3;
    static const QPointF POLY_FOOD[POLY_FOOD_N];
    static const QColor COLOUR_BACKGROUND;
    static const QColor COLOUR_ITEM;
    static const QColor COLOUR_ALPHA_BLACK;
    static const QString ICON_START;
    static const QImage ICON_CUP_WHITE;
    static const QImage ICON_CUP_ITEM;
    static const QString ICON_RESTART;
    static const QString ICON_SETTING;
    static const QImage IMG_GAME_OVER;
    static const QFont FONT_BRITANIC;
    static const QFont FONT_BRITANIC_LARGE;
private:
    std::stack<State> stk;
private:
    Snake knuth;
    int ocpy[M][N];
    std::queue<Direction> q;
    QPoint food;
    int tid, hi, interval;
private:
    QPushButton * btn_start;
    QPushButton * btn_restart;
    QPushButton * btn_setting;
private:
    void create_button(State state) {
        if (state == WELCOME) {
            btn_start = new QPushButton(QIcon((ICON_START)), "", this);
            btn_start->setGeometry(280, 330, 240, 240);
            btn_start->setFlat(true);
            btn_start->setIconSize(QSize(240, 240));
            QObject::connect(btn_start, SIGNAL(clicked()), this, SLOT(start()));
            btn_start->show();
        } else if (state == GAMEOVER) {
            btn_restart = new QPushButton(QIcon(ICON_RESTART), "", this);
            btn_setting = new QPushButton(QIcon(ICON_SETTING), "", this);
            btn_restart->setGeometry(250, 550, 120, 120);
            btn_restart->setFlat(true);
            btn_restart->setIconSize(QSize(120, 120));
            btn_setting->setGeometry(430, 550, 120, 120);
            btn_setting->setFlat(true);
            btn_setting->setIconSize(QSize(120, 120));
            QObject::connect(btn_restart, SIGNAL(clicked()), SLOT(restart()));
            QObject::connect(btn_setting, SIGNAL(clicked()), SLOT(setting()));
            btn_restart->show();
            btn_setting->show();
        } else if (state == PLAYING)
            ;
    }
    void destory_button(State state) {
        if (state == WELCOME)
            delete btn_start;
        else if (state == GAMEOVER) {
            delete btn_restart;
            delete btn_setting;
        }
    }
    bool valid(QPoint pos) {
        return pos.x() >= 0 && pos.x() < M && pos.y() >= 0 && pos.y() < N;
    }
    QPoint rnd_food() {
        QPoint ret(0, 0);
        int k = rand() % (M * N - knuth.len - 1);
        for (int i = 0; i < k; ++i) {
            do {
                ret += QPoint(0, 1);
                if (ret.y() >= N)
                    ret = QPoint(ret.x() + 1, 0);
            } while (ocpy[ret.x()][ret.y()]);
        }
        return ret;
    }
    void init_game() {
        knuth = Snake(M, N);
        memset(ocpy, 0, sizeof(ocpy));
        for (int i = 0; i < knuth.len; ++i)
            ocpy[knuth.shape[i].x()][knuth.shape[i].y()] = 1;
        while (!q.empty())
            q.pop();
        food = rnd_food();
        interval = 200;
        tid = startTimer(interval);
    }
private slots:
    void start() {
        stk.pop();
        destory_button(WELCOME);
        stk.push(PLAYING);
        create_button(PLAYING);
        init_game();
    }
    void restart() {
        stk.pop();
        destory_button(GAMEOVER);
        stk.push(PLAYING);
        create_button(PLAYING);
        init_game();
    }
    void setting() {
    }
public:
    QSnake() {
        setWindowTitle("QSnake");
        setGeometry(0, 0, WIDTH_MAIN, HEIGHT_MAIN);
        setFixedSize(size());
        srand(unsigned(time(nullptr)));
        hi = 0;
        stk.push(WELCOME);
        create_button(WELCOME);
    }
    ~QSnake() {
        while (!stk.empty()) {
            destory_button(stk.top());
            stk.pop();
        }
    }
private:
    void paintEvent(QPaintEvent *) {
        QPainter pt(this);
        pt.setPen(Qt::NoPen);
        pt.setBrush(COLOUR_BACKGROUND);
        pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN);
        if (stk.top() == WELCOME) {
            ;
        }
        else if (stk.top() == PLAYING) {
            pt.setBrush(COLOUR_ITEM);
            pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN - HEIGHT_PG);
            pt.save();
            pt.setBrush(Qt::white);
            pt.translate(20, 30);
            pt.scale(40, 40);
            pt.drawConvexPolygon(POLY_FOOD, POLY_FOOD_N);
            pt.restore();
            pt.drawImage(QRect(150, 33, 36, 36), ICON_CUP_WHITE);
            pt.save();
            pt.setPen(Qt::white);
            pt.setFont(FONT_BRITANIC);
            QString sc = QString("%1")
                    .arg(knuth.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(60, 65, sc);
            QString hs = QString("%1").arg(hi, 3, 10, QChar('0'));
            pt.drawText(188, 65, hs);
            pt.restore();
            pt.translate(0, HEIGHT_MAIN - HEIGHT_PG);
            pt.scale(ITEM_SIZE, ITEM_SIZE);
            for (int i = 0; i < knuth.len; ++i) {
                pt.save();
                pt.translate(knuth.shape[i]);
                pt.drawConvexPolygon(POLY_SNAKE, POLY_SNAKE_N);
                pt.restore();
            }
            pt.translate(food);
            pt.drawConvexPolygon(POLY_FOOD, POLY_FOOD_N);
        } else if (stk.top() == GAMEOVER) {
            pt.drawImage(QRect(200, 50, 400, 400), IMG_GAME_OVER);
            pt.save();
            pt.setBrush(COLOUR_ITEM);
            pt.translate(300, 360);
            pt.scale(60, 60);
            pt.drawConvexPolygon(POLY_FOOD, POLY_FOOD_N);
            pt.restore();
            pt.drawImage(QRect(302, 445, 56, 56), ICON_CUP_ITEM);
            pt.setPen(COLOUR_ITEM);
            pt.setFont(FONT_BRITANIC_LARGE);
            QString sc = QString("%1")
                    .arg(knuth.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(400, 415, sc);
            QString hs = QString("%1").arg(hi, 3, 10, QChar('0'));
            pt.drawText(400, 495, hs);
        }
    }
    void timerEvent(QTimerEvent * ev) {
        if (ev->timerId() != tid || stk.top() != PLAYING)
            return;
        if (!q.empty()) {
            knuth.dir = q.front();
            q.pop();
        }
        if (knuth.shape.back() + OFFSET[knuth.dir] == food) {
            knuth.shape.push_back(food);
            ++knuth.len;
            ocpy[food.x()][food.y()] = 1;
            if (knuth.len - Snake::INIT_LEN> hi)
                hi = knuth.len - Snake::INIT_LEN;
            food = rnd_food();
            if (interval > 80) {
                interval -= 2;
                killTimer(tid);
                tid = startTimer(interval);
            }
        } else {
            knuth.shape.push_back(knuth.shape.back() + OFFSET[knuth.dir]);
            bool crash = false;
            for (int i = 0; i < knuth.len - 1; ++i)
                if (knuth.shape.back() == knuth.shape[i])
                    crash = true;
            if (crash || !valid(knuth.shape.back())) {
                killTimer(tid);
                stk.pop();
                destory_button(PLAYING);
                stk.push(GAMEOVER);
                create_button(GAMEOVER);
                update();
                return;
            }
            ocpy[knuth.shape.front().x()][knuth.shape.front().y()] = 0;
            knuth.shape.pop_front();
            ocpy[knuth.shape.back().x()][knuth.shape.back().y()] = 1;
        }
        update();
    }
    void keyPressEvent(QKeyEvent * ev) {
        if (stk.top() != PLAYING)
            return;
        int dir;
        switch (ev->key()) {
            case Qt::Key_Left: case Qt::Key_A: dir = LEFT; break;
            case Qt::Key_Up: case Qt::Key_W: dir = UP; break;
            case Qt::Key_Down: case Qt::Key_S: dir = DOWN; break;
            case Qt::Key_Right: case Qt::Key_D: dir = RIGHT; break;
            default: dir = -1; break;
        }
        Direction cdir = q.empty() ? knuth.dir : q.front();
        if (dir != -1 && dir != cdir && dir != 3 - cdir)
            q.push(Direction(dir));
    }
};

#endif // QSNAKE_H

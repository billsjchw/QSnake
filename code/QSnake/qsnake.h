#ifndef QSNAKE_H
#define QSNAKE_H

#include <Qt>
#include <QWidget>
#include <QPainter>
#include <QtEvents>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QIcon>
#include <QFont>
#include <QString>
#include <QChar>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QPen>
#include <QSize>
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
    enum State {WELCOME, SINGLE, DOUBLE, GAMEOVER_SINGLE, GAMEOVER_DOUBLE, SETTING, PAUSE};
    enum Direction {LEFT, UP, DOWN, RIGHT};
    struct Snake {
        static const int INIT_LEN = 3;
        static const int INIT_INTV = 200;
        static const int MIN_INTV = 80;
        int len, interval;
        Direction dir;
        std::deque<QPoint> shape;
        Snake() {}
        Snake(int m, int n): len(INIT_LEN), interval(INIT_INTV) {
            QPoint head(m / 2, n / 2);
            dir = Direction(rand() % 4);
            for (int i = INIT_LEN - 1; i >= 0; --i)
                shape.push_back(head + OFFSET[3 - dir] * i);
        }
        Snake(int m, int n, int k): len(INIT_LEN), interval(INIT_INTV) {
            QPoint head(m / 3 * k, n / 2);
            dir = Direction(rand() % 4);
            for (int i = INIT_LEN - 1; i >= 0; --i)
                shape.push_back(head + OFFSET[3 - dir] * i);

        }
    };
    enum Name {KNUTH, LINUS};
private:
    static const int WIDTH_MAIN = 800;
    static const int WIDTH_MAIN_DOUBLE = 1200;
    static const int HEIGHT_MAIN = 900;
    static const int HEIGHT_PG = 800;
    static const int ITEM_SIZE = 40;
    static const int M = WIDTH_MAIN / ITEM_SIZE;
    static const int M_DOUBLE = WIDTH_MAIN_DOUBLE / ITEM_SIZE;
    static const int N = HEIGHT_PG / ITEM_SIZE;
    static const QPoint OFFSET[4];
    static const int POLY_SNAKE_N = 8;
    static const QPointF POLY_SNAKE[POLY_SNAKE_N];
    static const int POLY_FOOD_N = 3;
    static const QPointF POLY_FOOD[POLY_FOOD_N];
    static const QColor COLOUR_BACKGROUND;
    static const QColor COLOUR_ITEM;
    static const QColor COLOUR_ALPHA_BLACK;
    static const QColor COLOUR_BLUE;
    static const QColor COLOUR_RED;
    static const QString ICON_START;
    static const QImage ICON_CUP_WHITE;
    static const QImage ICON_CUP_ITEM;
    static const QString ICON_RESTART;
    static const QString ICON_SETTING;
    static const QString ICON_FINISH;
    static const QImage IMG_GAME_OVER;
    static const QFont FONT_BRITANIC;
    static const QFont FONT_BRITANIC_LARGE;
    static const QFont FONT_YAHEI;
    static const QFont FONT_YAHEI_LARGE;
private:
    std::stack<State> stk;
private:
    State pnum;
private:
    Snake knuth, linus;
    int ocpy[M_DOUBLE][N];
    std::queue<Direction> q[2];
    QPoint food;
    int tid[2], hi;
private:
    QPushButton * btn_start;
    QPushButton * btn_restart;
    QPushButton * btn_setting;
    QPushButton * btn_finish;
    QButtonGroup * btg_pnum;
    QRadioButton * btn_single;
    QRadioButton * btn_double;
private:
    void resize_fixed(int width, int height) {
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        resize(width, height);
        setFixedSize(size());
    }
    void create_button(State state) {
        if (state == WELCOME) {
            btn_start = new QPushButton(QIcon((ICON_START)), "", this);
            btn_start->setGeometry(280, 330, 240, 240);
            btn_start->setFlat(true);
            btn_start->setIconSize(QSize(240, 240));
            connect(btn_start, SIGNAL(clicked()), this, SLOT(start()));
        } else if (state == GAMEOVER_SINGLE || state == GAMEOVER_DOUBLE) {
            btn_restart = new QPushButton(QIcon(ICON_RESTART), "", this);
            btn_setting = new QPushButton(QIcon(ICON_SETTING), "", this);
            btn_restart->setGeometry(250, 550, 120, 120);
            btn_restart->setFlat(true);
            btn_restart->setIconSize(QSize(120, 120));
            btn_setting->setGeometry(430, 550, 120, 120);
            btn_setting->setFlat(true);
            btn_setting->setIconSize(QSize(120, 120));
            connect(btn_restart, SIGNAL(clicked()), SLOT(restart()));
            connect(btn_setting, SIGNAL(clicked()), SLOT(setting()));
        } else if (state == SETTING) {
            btn_single = new QRadioButton("1", this);
            btn_double = new QRadioButton("2", this);
            btn_single->setFont(FONT_YAHEI);
            btn_double->setFont(FONT_YAHEI);
            if (pnum == SINGLE)
                btn_single->setChecked(true);
            else
                btn_double->setChecked(true);
            btn_single->setGeometry(400, 270, 50, 50);
            btn_double->setGeometry(500, 270, 50, 50);
            btg_pnum = new QButtonGroup(this);
            btg_pnum->addButton(btn_single, SINGLE);
            btg_pnum->addButton(btn_double, DOUBLE);
            btn_finish = new QPushButton(QIcon(ICON_FINISH), "", this);
            btn_finish->setGeometry(340, 550, 120, 120);
            btn_finish->setFlat(true);
            btn_finish->setIconSize(QSize(120, 120));
            connect(btn_finish, SIGNAL(clicked()), SLOT(finish_setting()));
        }
        show_button(state);
    }
    void destory_button(State state) {
        if (state == WELCOME)
            delete btn_start;
        else if (state == GAMEOVER_SINGLE || state == GAMEOVER_DOUBLE) {
            delete btn_restart;
            delete btn_setting;
        } else if (state == SETTING) {
            delete btg_pnum;
            delete btn_single;
            delete btn_double;
            delete btn_finish;
        }
    }
    void hide_button(State state) {
        if (state == GAMEOVER_SINGLE || state == GAMEOVER_DOUBLE) {
            btn_restart->hide();
            btn_setting->hide();
        }
    }
    void show_button(State state) {
        if (state == WELCOME)
            btn_start->show();
        else if (state == GAMEOVER_SINGLE || state == GAMEOVER_DOUBLE) {
            btn_restart->show();
            btn_setting->show();
        } else if (state == SETTING) {
            btn_single->show();
            btn_double->show();
            btn_finish->show();
        }
    }
    bool valid(QPoint pos) {
        int m = pnum == SINGLE ? M : M_DOUBLE;
        return pos.x() >= 0 && pos.x() < m && pos.y() >= 0 && pos.y() < N
                && !ocpy[pos.x()][pos.y()];
    }
    QPoint rnd_food() {
        QPoint ret(0, 0);
        int k;
        if (pnum == SINGLE)
            k = rand() % (M * N - knuth.len - 1);
        else
            k = rand() % (M_DOUBLE * N - knuth.len - linus.len - 1);
        for (int i = 0; i < k; ++i)
            do {
                ret += QPoint(0, 1);
                if (ret.y() >= N)
                    ret = QPoint(ret.x() + 1, 0);
            } while (ocpy[ret.x()][ret.y()]);
        return ret;
    }
    void init_game() {
        if (pnum == SINGLE)
            knuth = Snake(M, N);
        else {
            knuth = Snake(M_DOUBLE, N, 1);
            linus = Snake(M_DOUBLE, N, 2);
        }
        memset(ocpy, 0, sizeof(ocpy));
        for (int i = 0; i < knuth.len; ++i)
            ocpy[knuth.shape[i].x()][knuth.shape[i].y()] = 1;
        while (!q[KNUTH].empty())
            q[KNUTH].pop();
        if (pnum == DOUBLE) {
            for (int i = 0; i < linus.len; ++i)
                ocpy[linus.shape[i].x()][linus.shape[i].y()] = 1;
            while (!q[LINUS].empty())
                q[LINUS].pop();
        }
        food = rnd_food();
        if (pnum == DOUBLE)
            resize_fixed(WIDTH_MAIN_DOUBLE, HEIGHT_MAIN);
        update();
        tid[KNUTH] = startTimer(knuth.interval);
        if (pnum == DOUBLE)
            tid[LINUS] = startTimer(linus.interval);
    }
private slots:
    void start() {
        destory_button(WELCOME);
        stk.pop();
        stk.push(pnum);
        create_button(pnum);
        init_game();
    }
    void restart() {
        destory_button(stk.top());
        stk.pop();
        stk.push(pnum);
        create_button(pnum);
        init_game();
    }
    void setting() {
        hide_button(stk.top());
        stk.push(SETTING);
        create_button(SETTING);
        update();
    }
    void finish_setting() {
        pnum = State(btg_pnum->checkedId());
        destory_button(SETTING);
        stk.pop();
        show_button(stk.top());
        update();
    }
public:
    QSnake(): pnum(SINGLE), hi(0) {
        setWindowTitle("QSnake");
        resize_fixed(WIDTH_MAIN, HEIGHT_MAIN);
        srand(unsigned(time(nullptr)));
        create_button(WELCOME);
        stk.push(WELCOME);
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
        if (stk.top() == WELCOME) {
            pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN);;
        }
        else if (stk.top() == SINGLE) {
            pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN);
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
        } else if (stk.top() == GAMEOVER_SINGLE) {
            pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN);
            pt.drawImage(QRect(204, 50, 400, 400), IMG_GAME_OVER);
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
        } else if (stk.top() == GAMEOVER_DOUBLE) {
            pt.drawRect(0, 0, WIDTH_MAIN, HEIGHT_MAIN);
            pt.drawImage(QRect(204, 50, 400, 400), IMG_GAME_OVER);
            pt.setBrush(COLOUR_RED);
            pt.drawEllipse(302, 367, 54, 54);
            pt.setBrush(COLOUR_BLUE);
            pt.drawEllipse(302, 450, 54, 54);
            pt.setPen(COLOUR_ITEM);
            pt.setFont(FONT_BRITANIC_LARGE);
            QString ks = QString("%1")
                    .arg(knuth.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(400, 415, ks);
            QString ls = QString("%1")
                    .arg(linus.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(400, 495, ls);
        } else if (stk.top() == DOUBLE) {
            pt.drawRect(0, 0, WIDTH_MAIN_DOUBLE, HEIGHT_MAIN);
            pt.setBrush(COLOUR_RED);
            pt.drawRect(0, 0, WIDTH_MAIN_DOUBLE / 2, HEIGHT_MAIN - HEIGHT_PG);
            pt.setBrush(COLOUR_BLUE);
            pt.drawRect(WIDTH_MAIN_DOUBLE / 2, 0, WIDTH_MAIN_DOUBLE / 2, HEIGHT_MAIN - HEIGHT_PG);
            pt.save();
            pt.setPen(Qt::white);
            pt.setFont(FONT_BRITANIC);
            QString ks = QString("%1")
                    .arg(knuth.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(30, 65, ks);
            QString ls = QString("%1")
                    .arg(linus.len - Snake::INIT_LEN, 3, 10, QChar('0'));
            pt.drawText(1100, 65, ls);
            pt.restore();
            pt.translate(0, HEIGHT_MAIN - HEIGHT_PG);
            pt.setBrush(COLOUR_RED);
            pt.scale(ITEM_SIZE, ITEM_SIZE);
            for (int i = 0; i < knuth.len; ++i) {
                pt.save();
                pt.translate(knuth.shape[i]);
                pt.drawConvexPolygon(POLY_SNAKE, POLY_SNAKE_N);
                pt.restore();
            }
            pt.setBrush(COLOUR_BLUE);
            for (int i = 0; i < linus.len; ++i) {
                pt.save();
                pt.translate(linus.shape[i]);
                pt.drawConvexPolygon(POLY_SNAKE, POLY_SNAKE_N);
                pt.restore();
            }
            pt.setBrush(COLOUR_ITEM);
            pt.translate(food);
            pt.drawConvexPolygon(POLY_FOOD, POLY_FOOD_N);
        } else if (stk.top() == SETTING) {
            pt.drawRect(0, 0, WIDTH_MAIN_DOUBLE, HEIGHT_MAIN);
            pt.setPen(Qt::black);
            pt.setFont(FONT_YAHEI_LARGE);
            pt.drawText(270, 200, "Setting");
            pt.setFont(FONT_YAHEI);
            pt.drawText(260, 303, "Players:");
        }
    }
    void timerEvent(QTimerEvent * ev) {
        if (stk.top() != SINGLE && stk.top() != DOUBLE)
            return;
        Snake * snake;
        Name name;
        if (ev->timerId() == tid[KNUTH]) {
            snake = &knuth;
            name = KNUTH;
        } else if (pnum == DOUBLE && ev->timerId() == tid[LINUS]) {
            snake = &linus;
            name = LINUS;
        } else
            return;
        if (!q[name].empty()) {
            snake->dir = q[name].front();
            q[name].pop();
        }
        if (snake->shape.back() + OFFSET[snake->dir] == food) {
            snake->shape.push_back(food);
            ++snake->len;
            if (pnum == SINGLE && snake->len - Snake::INIT_LEN> hi)
                hi = snake->len - Snake::INIT_LEN;
            food = rnd_food();
            if (snake->interval > Snake::MIN_INTV) {
                snake->interval -= 2;
                killTimer(tid[name]);
                tid[name] = startTimer(snake->interval);
            }
        } else {
            snake->shape.push_back(snake->shape.back() + OFFSET[snake->dir]);
            ocpy[snake->shape.front().x()][snake->shape.front().y()] = 0;
            snake->shape.pop_front();
        }
        if (!valid(snake->shape.back())) {
            killTimer(tid[KNUTH]);
            if (pnum == DOUBLE)
                killTimer(tid[LINUS]);
            stk.pop();
            destory_button(pnum);
            stk.push(pnum == SINGLE ? GAMEOVER_SINGLE : GAMEOVER_DOUBLE);
            resize_fixed(WIDTH_MAIN, HEIGHT_MAIN);
            create_button(pnum == SINGLE ? GAMEOVER_SINGLE : GAMEOVER_DOUBLE);
        } else
            ocpy[snake->shape.back().x()][snake->shape.back().y()] = 1;
        update();
    }
    void keyPressEvent(QKeyEvent * ev) {
        if (stk.top() != SINGLE && stk.top() != DOUBLE)
            return;
        int dir;
        Snake * snake = &knuth;
        Name name = KNUTH;
        switch (ev->key()) {
            case Qt::Key_A: snake = &linus; name = LINUS;
            case Qt::Key_Left: dir = LEFT; break;
            case Qt::Key_W: snake = &linus; name = LINUS;
            case Qt::Key_Up: dir = UP; break;
            case Qt::Key_S: snake = &linus; name = LINUS;
            case Qt::Key_Down: dir = DOWN; break;
            case Qt::Key_D: snake = &linus; name = LINUS;
            case Qt::Key_Right: dir = RIGHT; break;
            default: dir = -1; break;
        }
        if (pnum == SINGLE) {
            snake = &knuth;
            name = KNUTH;
        }
        Direction cdir = q[name].empty() ? snake->dir : q[name].back();
        if (dir != -1 && dir != cdir && dir != 3 - cdir)
            q[name].push(Direction(dir));
    }
};

#endif // QSNAKE_H

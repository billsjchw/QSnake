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
#include <QFile>
#include <QDataStream>
#include <QComboBox>
#include <deque>
#include <stack>
#include <queue>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

class QSnake: public QWidget {
Q_OBJECT
private:
    enum State {WELCOME, SINGLE, DOUBLE, GAMEOVER_SINGLE, GAMEOVER_DOUBLE, SETTING, PAUSE};
    enum Direction {LEFT, UP, DOWN, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, ORIGIN};
    struct Snake {
        static const int INIT_LEN = 3;
        static const int INIT_INTV = 200;
        static const int MIN_INTV = 80;
        int len, interval;
        Direction dir;
        std::deque<QPoint> shape;
        Snake() {}
        Snake(QPoint head, Direction dir): len(INIT_LEN), interval(INIT_INTV), dir(dir) {
            for (int i = INIT_LEN - 1; i >= 0; --i)
                shape.push_back(head + OFFSET[3 - dir] * i);
        }
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
    enum BlockType {NOBLOCK, BLOCK, RBLOCK};
private:
    static const int WIDTH_MAIN = 800;
    static const int WIDTH_MAIN_DOUBLE = 1200;
    static const int HEIGHT_MAIN = 900;
    static const int HEIGHT_PG = 800;
    static const int ITEM_SIZE = 40;
    static const int MAP_NUM_SINGLE = 2;
    static const int MAP_NUM_DOUBLE = 2;
    static const int M = WIDTH_MAIN / ITEM_SIZE;
    static const int M_DOUBLE = WIDTH_MAIN_DOUBLE / ITEM_SIZE;
    static const int N = HEIGHT_PG / ITEM_SIZE;
    static const int RBLOCK_INTV = 8000;
    static const int INIT_RBCNT;
    static const QPoint OFFSET[9];
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
    static const QImage ICON_BLOCK;
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
    bool wall, landform, rblock;
private:
    Snake knuth, linus;
    int ocpy[M_DOUBLE][N];
    BlockType block[M_DOUBLE][N];
    int bcnt, rbcnt;
    std::queue<Direction> q[2];
    QPoint food;
    int tid[2], hi, tid_rblock;
private:
    QPushButton * btn_start;
    QPushButton * btn_restart;
    QPushButton * btn_setting;
    QPushButton * btn_finish;
    QButtonGroup * btg_pnum;
    QRadioButton * btn_single;
    QRadioButton * btn_double;
    QButtonGroup * btg_wall;
    QRadioButton * btn_wall;
    QRadioButton * btn_nowall;
    QButtonGroup * btg_block;
    QRadioButton * btn_block;
    QRadioButton * btn_noblock;
    QButtonGroup * btg_rblock;
    QRadioButton * btn_rblock;
    QRadioButton * btn_norblock;
    QComboBox * cbb_rbcnt;
private:
    void resize_fixed(int width, int height) {
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        resize(width, height);
        setFixedSize(size());
    }
    void create_rbutton(QRadioButton * & pb1, QRadioButton * & pb2, QButtonGroup * & pbg,
                        int x1, int x2, int y, bool flag,
                        const QString & txt1, const QString & txt2,
                        int id1, int id2) {
        pb1 = new QRadioButton(txt1, this);
        pb2 = new QRadioButton(txt2, this);
        pb1->setFont(FONT_YAHEI);
        pb2->setFont(FONT_YAHEI);
        if (flag)
            pb1->setChecked(true);
        else
            pb2->setChecked(true);
        pb1->setGeometry(x1, y, 200, 20);
        pb2->setGeometry(x2, y, 200, 20);
        pbg = new QButtonGroup(this);
        pbg->addButton(pb1, id1);
        pbg->addButton(pb2, id2);
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
            create_rbutton(btn_single, btn_double, btg_pnum,
                           400, 500, 285, pnum == SINGLE, "1", "2", SINGLE, DOUBLE);
            create_rbutton(btn_wall, btn_nowall, btg_wall,
                           400, 540, 330, wall, "Disable", "Enable", true, false);
            create_rbutton(btn_noblock, btn_block, btg_block,
                           400, 540, 375, !landform, "Disable", "Enable", false, true);
            create_rbutton(btn_norblock, btn_rblock, btg_rblock,
                           400, 540, 420, !rblock, "Disable", "Enable", false, true);
            cbb_rbcnt = new QComboBox(this);
            if (!rblock)
                cbb_rbcnt->setEnabled(false);
            cbb_rbcnt->addItem("1");
            cbb_rbcnt->addItem("2");
            cbb_rbcnt->addItem("3");
            if (pnum == DOUBLE) {
                cbb_rbcnt->addItem("4");
                cbb_rbcnt->addItem("5");
            }
            cbb_rbcnt->setFont(FONT_YAHEI);
            cbb_rbcnt->setCurrentIndex(rbcnt - 1);
            cbb_rbcnt->setGeometry(400, 460, 200, 40);
            btn_finish = new QPushButton(QIcon(ICON_FINISH), "", this);
            btn_finish->setGeometry(340, 550, 120, 120);
            btn_finish->setFlat(true);
            btn_finish->setIconSize(QSize(120, 120));
            connect(btn_single, SIGNAL(clicked()), SLOT(single_player()));
            connect(btn_double, SIGNAL(clicked()), SLOT(double_player()));
            connect(btn_norblock, SIGNAL(clicked()), SLOT(set_norblock()));
            connect(btn_rblock, SIGNAL(clicked()), SLOT(set_rblock()));
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
            delete btn_wall;
            delete btn_nowall;
            delete btn_block;
            delete btn_noblock;
            delete btn_rblock;
            delete btn_norblock;
            delete cbb_rbcnt;
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
            btn_wall->show();
            btn_nowall->show();
            btn_block->show();
            btn_noblock->show();
            btn_rblock->show();
            btn_norblock->show();
            cbb_rbcnt->show();
            btn_finish->show();
        }
    }
    bool valid(QPoint pos) {
        int m = pnum == SINGLE ? M : M_DOUBLE;
        return pos.x() >= 0 && pos.x() < m && pos.y() >= 0 && pos.y() < N
                && !ocpy[pos.x()][pos.y()] && !block[pos.x()][pos.y()];
    }
    bool valid_around(QPoint pos) {
        for (int i = 0; i < 9; ++i) {
            QPoint npos = pos + OFFSET[i];
            if (ocpy[npos.x()][npos.y()] || block[npos.x()][npos.y()])
                return false;
        }
        return true;
    }
    QPoint rnd_food() {
        QPoint ret(0, -1);
        int k;
        if (pnum == SINGLE)
            k = rand() % (M * N - knuth.len - bcnt - rbcnt) + 1;
        else
            k = rand() % (M_DOUBLE * N - knuth.len - linus.len - bcnt - rbcnt) + 1;
        for (int i = 0; i < k; ++i)
            do {
                ret += QPoint(0, 1);
                if (ret.y() >= N)
                    ret = QPoint(ret.x() + 1, 0);
            } while (!valid(ret));
        return ret;
    }
    void rnd_block() {
        int m = pnum == SINGLE ? M : M_DOUBLE;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < N; ++j)
                if (block[i][j] == RBLOCK)
                    block[i][j] = NOBLOCK;
        for (int t = 0; t < rbcnt; ++t) {
            std::vector<QPoint> space;
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < N; ++j)
                    if (valid_around(QPoint(i, j)))
                        space.push_back(QPoint(i, j));
            QPoint pos = space[rand() % space.size()];
            block[pos.x()][pos.y()] = RBLOCK;
        }
    }
    void init_game() {
        memset(block, 0, sizeof(block));
        if (landform) {
            QFile file;
            QDataStream in(&file);
            int x, y, dir;
            if (pnum == SINGLE) {
                QString no = QString("%1")
                        .arg(rand() % MAP_NUM_SINGLE, 2, 10, QChar('0'));
                file.setFileName(QString(":/map/single") + no + QString(".map"));
                file.open(QIODevice::ReadOnly);
                in.readRawData((char *) &x, sizeof(x));
                in.readRawData((char *) &y, sizeof(y));
                in.readRawData((char *) &dir, sizeof(dir));
                knuth = Snake(QPoint(x, y), Direction(dir));
            } else {
                QString no = QString("%1")
                        .arg(rand() % MAP_NUM_DOUBLE, 2, 10, QChar('0'));
                file.setFileName(QString(":/map/double") + no + QString(".map"));
                file.open(QIODevice::ReadOnly);
                in.readRawData((char *) &x, sizeof(x));
                in.readRawData((char *) &y, sizeof(y));
                in.readRawData((char *) &dir, sizeof(dir));
                knuth = Snake(QPoint(x, y), Direction(dir));
                in.readRawData((char *) &x, sizeof(x));
                in.readRawData((char *) &y, sizeof(y));
                in.readRawData((char *) &dir, sizeof(dir));
                linus = Snake(QPoint(x, y), Direction(dir));
            }
            bcnt = 0;
            while (in.status() != QDataStream::ReadPastEnd) {
                in.readRawData((char *) &x, sizeof(x));
                in.readRawData((char *) &y, sizeof(y));
                block[x][y] = BLOCK;
                ++bcnt;
            }
            file.close();
        } else if (pnum == SINGLE)
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
        if (rblock)
            rnd_block();
        food = rnd_food();
        if (pnum == DOUBLE)
            resize_fixed(WIDTH_MAIN_DOUBLE, HEIGHT_MAIN);
        update();
        tid[KNUTH] = startTimer(knuth.interval);
        if (pnum == DOUBLE)
            tid[LINUS] = startTimer(linus.interval);
        if (rblock)
            tid_rblock = startTimer(RBLOCK_INTV);
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
        wall = btg_wall->checkedId();
        landform = btg_block->checkedId();
        rblock = btg_rblock->checkedId();
        rbcnt = cbb_rbcnt->currentIndex() + 1;
        destory_button(SETTING);
        stk.pop();
        show_button(stk.top());
        update();
    }
    void single_player() {
        if (cbb_rbcnt->count() == 5) {
            cbb_rbcnt->setCurrentIndex(2);
            cbb_rbcnt->removeItem(3);
            cbb_rbcnt->removeItem(3);
        }
    }
    void double_player() {
        if (cbb_rbcnt->count() == 3) {
            cbb_rbcnt->addItem("4");
            cbb_rbcnt->addItem("5");
        }
    }
    void set_norblock() {
        cbb_rbcnt->setEnabled(false);
    }
    void set_rblock() {
        cbb_rbcnt->setEnabled(true);
    }
public:
    QSnake():
        pnum(SINGLE), wall(true), landform(false), rblock(false), rbcnt(3), hi(0) {
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
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    if (block[i][j] == BLOCK)
                        pt.drawRect(i, j, 1, 1);
                    else if (block[i][j] == RBLOCK)
                        pt.drawImage(QRect(i, j, 1, 1), ICON_BLOCK);
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
            for (int i = 0; i < M_DOUBLE; ++i)
                for (int j = 0; j < N; ++j)
                    if (block[i][j] == BLOCK)
                        pt.drawRect(i, j, 1, 1);
                    else if (block[i][j] == RBLOCK)
                        pt.drawImage(QRect(i, j, 1, 1), ICON_BLOCK);
            pt.translate(food);
            pt.drawConvexPolygon(POLY_FOOD, POLY_FOOD_N);
        } else if (stk.top() == SETTING) {
            pt.drawRect(0, 0, WIDTH_MAIN_DOUBLE, HEIGHT_MAIN);
            pt.setPen(Qt::black);
            pt.setFont(FONT_YAHEI_LARGE);
            pt.drawText(270, 200, "Setting");
            pt.setFont(FONT_YAHEI);
            pt.drawText(260, 303, "Players:");
            pt.drawText(136, 348, "Go Through Walls:");
            pt.drawText(162, 393, "Initial Landform:");
            pt.drawText(166, 437, "Random Blocks:");
            pt.drawText(64, 490, "Random Blocks Number:");
        }
    }
    void timerEvent(QTimerEvent * ev) {
        if (stk.top() != SINGLE && stk.top() != DOUBLE)
            return;
        if (rblock && ev->timerId() == tid_rblock) {
            rnd_block();
            return;
        }
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
            if (!wall) {
                int m = pnum == SINGLE ? M : M_DOUBLE;
                snake->shape.back().setX((snake->shape.back().x() + m) % m);
                snake->shape.back().setY((snake->shape.back().y() + N) % N);
            }
            ocpy[snake->shape.front().x()][snake->shape.front().y()] = 0;
            snake->shape.pop_front();
        }
        if (!valid(snake->shape.back())) {
            killTimer(tid[KNUTH]);
            if (pnum == DOUBLE)
                killTimer(tid[LINUS]);
            if (rblock)
                killTimer(tid_rblock);
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

#ifndef GAME_WINDOW_DOUBLE_H
#define GAME_WINDOW_DOUBLE_H

#include "game_window.h"
#include "game_double.h"
#include <QPaintEvent>
#include <QKeyEvent>
#include "pause_window.h"
#include <QColor>
#include "ui_game_window_double.h"

class GameWindowDouble: public GameWindow {
    Q_OBJECT
private:
    static const int WIDTH = 1200;
    static const int HEIGHT = 800;
    static const int TITLE_HEIGHT = 100;
    static const QColor COLOR_RED;
    static const QColor COLOR_BLUE;
private:
    Ui::GameWindowDouble * ui;
    GameDouble * gameDouble;
    PauseWindow * pauseWindow;
public:
    GameWindowDouble();
    ~GameWindowDouble();
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent * event);
    void pause();
private slots:
    void resume();
    void handleGameOver();
};

#endif // GAME_WINDOW_DOUBLE_H

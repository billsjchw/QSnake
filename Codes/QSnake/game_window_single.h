#ifndef GAME_WINDOW_SINGLE_H
#define GAME_WINDOW_SINGLE_H

#include "game_window.h"
#include <QPixmap>
#include "game_single.h"
#include "pause_window.h"
#include <QColor>
#include "ui_game_window_single.h"

class GameWindowSingle: public GameWindow {
    Q_OBJECT
private:
    static const int WIDTH = 800;
    static const int HEIGHT = 800;
    static const int TITLE_HEIGHT = 100;
    static const QColor COLOR_BROWN;
private:
    Ui::GameWindowSingle * ui;
    GameSingle * gameSingle;
    PauseWindow * pauseWindow;
public:
    GameWindowSingle();
    ~GameWindowSingle();
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent * event);
    void pause();
private slots:
    void resume(int automaticFlag);
    void handleGameOver();
};

#endif // GAME_WINDOW_SINGLE_H

#ifndef GAME_OVER_WINDOW_SINGLE_H
#define GAME_OVER_WINDOW_SINGLE_H

#include "game_over_window.h"
#include "ui_game_over_window_single.h"

class GameOverWindowSingle: public GameOverWindow {
private:
    Ui::GameOverWindowSingle * ui;
public:
    GameOverWindowSingle(int score);
    ~GameOverWindowSingle();
};

#endif // GAME_OVER_WINDOW_SINGLE_H

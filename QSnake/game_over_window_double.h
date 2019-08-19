#ifndef GAME_OVER_WINDOW_DOUBLE_H
#define GAME_OVER_WINDOW_DOUBLE_H

#include "game_over_window.h"
#include "ui_game_over_window_double.h"

class GameOverWindowDouble: public GameOverWindow {
private:
    Ui::GameOverWindowDouble * ui;
public:
    GameOverWindowDouble(int scoreRed, int scoreBlue);
    ~GameOverWindowDouble();
};

#endif // GAME_OVER_WINDOW_DOUBLE_H

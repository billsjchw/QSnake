#ifndef START_WINDOW_H
#define START_WINDOW_H

#include "game_over_window.h"
#include "ui_start_window.h"

class StartWindow: public GameOverWindow {
private:
    Ui::StartWindow * ui;
public:
    StartWindow();
    ~StartWindow();
};

#endif // START_WINDOW_H

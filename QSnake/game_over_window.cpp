#include "game_over_window.h"
#include "configuration.h"
#include "game_window_single.h"
#include "game_window_double.h"

GameOverWindow::GameOverWindow() {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);
    settingWindow = new SettingWindow(this);
}

void GameOverWindow::setting() {
    settingWindow->open();
}

void GameOverWindow::restart() {
    if (conf.player == Configuration::SINGLE) {
        GameWindowSingle * gameWindowSingle = new GameWindowSingle;
        gameWindowSingle->show();
    } else {
        GameWindowDouble * gameWindowDouble = new GameWindowDouble;
        gameWindowDouble->show();
    }
    close();
}

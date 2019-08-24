#ifndef GAME_OVER_WINDOW_H
#define GAME_OVER_WINDOW_H

#include <QWidget>
#include "setting_window.h"

class GameOverWindow: public QWidget {
    Q_OBJECT
protected:
    SettingWindow * settingWindow;
public:
    GameOverWindow();
protected slots:
    void setting();
    void restart();
};

#endif // GAME_OVER_WINDOW_H

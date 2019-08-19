#include <QApplication>
#include "game_window_single.h"
#include "game_window_double.h"
#include "start_window.h"

int main(int argc, char * argv[]) {
    QApplication qSnake(argc, argv);

//    GameWindowSingle * gameWindowSingle = new GameWindowSingle;
//    gameWindowSingle->show();
//    GameWindowDouble * gameWindowDouble = new GameWindowDouble;
//    gameWindowDouble->show();
    StartWindow * startWindow = new StartWindow;
    startWindow->show();

    return qSnake.exec();
}

#include <QApplication>
#include <QFontDatabase>
#include "start_window.h"

int main(int argc, char * argv[]) {
    QApplication qSnake(argc, argv);
    QFontDatabase::addApplicationFont(":/font/consola.ttf");

    StartWindow * startWindow = new StartWindow;
    startWindow->show();

    return qSnake.exec();
}

#include <QApplication>
#include <QFontDatabase>
#include <QString>
#include <iostream>
#include "qsnake.h"

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":/font/msyhbd.ttf");
    QFontDatabase::addApplicationFont(":/font/BRITANIC.TTF");

    QSnake qsnake;
    qsnake.show();

    return app.exec();
}

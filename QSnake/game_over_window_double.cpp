#include "game_over_window_double.h"
#include <QString>

GameOverWindowDouble::GameOverWindowDouble(int scoreRed, int scoreBlue) {
    ui = new Ui::GameOverWindowDouble;
    ui->setupUi(this);
    connect(ui->setting, SIGNAL(clicked()), this, SLOT(setting()));
    connect(ui->restart, SIGNAL(clicked()), this, SLOT(restart()));
    ui->scoreRed->setText(QString("%1").arg(scoreRed, 3, 10, QChar('0')));
    ui->scoreBlue->setText(QString("%1").arg(scoreBlue, 3, 10, QChar('0')));
}

GameOverWindowDouble::~GameOverWindowDouble() {
    delete ui;
}

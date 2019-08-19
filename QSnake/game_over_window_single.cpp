#include "game_over_window_single.h"
#include <QString>
#include <QChar>

GameOverWindowSingle::GameOverWindowSingle(int score) {
    ui = new Ui::GameOverWindowSingle;
    ui->setupUi(this);
    connect(ui->setting, SIGNAL(clicked()), this, SLOT(setting()));
    connect(ui->restart, SIGNAL(clicked()), this, SLOT(restart()));
    ui->score->setText(QString("%1").arg(score, 3, 10, QChar('0')));
}

GameOverWindowSingle::~GameOverWindowSingle() {
    delete ui;
}

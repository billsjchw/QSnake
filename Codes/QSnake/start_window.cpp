#include "start_window.h"

StartWindow::StartWindow() {
    ui = new Ui::StartWindow;
    ui->setupUi(this);
    connect(ui->start, SIGNAL(clicked()), this, SLOT(restart()));
    connect(ui->setting, SIGNAL(clicked()), this, SLOT(setting()));
}

StartWindow::~StartWindow() {
    delete ui;
}

#include "pause_window.h"
#include <QKeyEvent>
#include <QCloseEvent>

PauseWindow::PauseWindow(QWidget * parent) {
    setParent(parent);
    setWindowFlag(Qt::Window);
    ui = new Ui::PauseWindow;
    ui->setupUi(this);
    connect(ui->resume, SIGNAL(clicked()), this, SLOT(close()));
}

PauseWindow::~PauseWindow() {
    delete ui;
}

void PauseWindow::disableAutomatic() {
    ui->automatic->setEnabled(false);
}

void PauseWindow::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_P)
        close();
}

void PauseWindow::closeEvent(QCloseEvent * event) {
    event->ignore();
    hide();
    if (ui->automatic->checkState() == Qt::Unchecked)
        emit finished(0);
    else
        emit finished(1);
}

#ifndef PAUSE_WINDOW_H
#define PAUSE_WINDOW_H

#include <QDialog>
#include "ui_pause_window.h"

class PauseWindow: public QDialog {
private:
    Ui::PauseWindow * ui;
public:
    PauseWindow(QWidget * parent);
    ~PauseWindow();
    void disableAutomatic();
protected:
    void keyPressEvent(QKeyEvent * event);
    void closeEvent(QCloseEvent * event);
};

#endif // PAUSE_WINDOW_H

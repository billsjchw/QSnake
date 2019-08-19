#ifndef SETTING_WINDOW_H
#define SETTING_WINDOW_H

#include <QDialog>
#include <QShowEvent>
#include "ui_setting_window.h"

class SettingWindow: public QDialog {
    Q_OBJECT
private:
    Ui::SettingWindow * ui;
public:
    SettingWindow(QWidget * parent);
    ~SettingWindow();
private:
    void showEvent(QShowEvent *);
    void setBlockNumChoices();
private slots:
    void commitSetting();
    void changeBlockNumChoices();
};

#endif // SETTING_WINDOW_H

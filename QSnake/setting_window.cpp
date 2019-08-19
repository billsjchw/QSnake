#include "setting_window.h"
#include "configuration.h"
#include <QString>

SettingWindow::SettingWindow(QWidget * parent) {
    setParent(parent);
    setWindowFlag(Qt::Window);
    ui = new Ui::SettingWindow;
    ui->setupUi(this);
    connect(ui->playerSingle, SIGNAL(clicked()), this, SLOT(changeBlockNumChoices()));
    connect(ui->playerDouble, SIGNAL(clicked()), this, SLOT(changeBlockNumChoices()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(commitSetting()));
}

SettingWindow::~SettingWindow() {
    delete ui;
}

void SettingWindow::showEvent(QShowEvent *) {
    if (conf.player == Configuration::SINGLE)
        ui->playerSingle->setChecked(true);
    else if (conf.player == Configuration::DOUBLE)
        ui->playerDouble->setChecked(true);
    if (conf.through == Configuration::DISABLE)
        ui->throughDisable->setChecked(true);
    else
        ui->throughEnable->setChecked(true);
    if (conf.bricks == Configuration::DISABLE)
        ui->bricksDisable->setChecked(true);
    else
        ui->bricksEnable->setChecked(true);
    if (conf.star == Configuration::DISABLE)
        ui->starDisable->setChecked(true);
    else
        ui->starEnable->setChecked(true);
    setBlockNumChoices();
    ui->blockNum->setCurrentText(QString("%1").arg(conf.blockNum));
}

void SettingWindow::commitSetting() {
    if (ui->playerSingle->isChecked())
        conf.player = Configuration::SINGLE;
    else
        conf.player = Configuration::DOUBLE;
    if (ui->throughDisable->isChecked())
        conf.through = Configuration::DISABLE;
    else
        conf.through = Configuration::ENABLE;
    if (ui->bricksDisable->isChecked())
        conf.bricks = Configuration::DISABLE;
    else
        conf.bricks = Configuration::ENABLE;
    if (ui->starDisable->isChecked())
        conf.star = Configuration::DISABLE;
    else
        conf.star = Configuration::ENABLE;
    conf.blockNum = ui->blockNum->currentText().toInt();
    close();
}

void SettingWindow::setBlockNumChoices() {
    int maxBlockNum;
    if (ui->playerSingle->isChecked())
        maxBlockNum = Configuration::MAX_BLOCK_NUM_SINGLE;
    else
        maxBlockNum = Configuration::MAX_BLOCK_NUM_DOUBLE;
    ui->blockNum->clear();
    for (int i = 0; i <= maxBlockNum; ++i)
        ui->blockNum->addItem(QString("%1").arg(i));
}

void SettingWindow::changeBlockNumChoices() {
    int maxBlockNum;
    if (ui->playerSingle->isChecked())
        maxBlockNum = Configuration::MAX_BLOCK_NUM_SINGLE;
    else
        maxBlockNum = Configuration::MAX_BLOCK_NUM_DOUBLE;
    int blockNum = ui->blockNum->currentText().toInt();
    if (blockNum > maxBlockNum)
        blockNum = maxBlockNum;
    setBlockNumChoices();
    ui->blockNum->setCurrentText(QString("%1").arg(blockNum));
}

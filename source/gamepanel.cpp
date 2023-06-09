#include "gamepanel.h"
#include "ui_mainwindow.h"

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

GamePanel::~GamePanel()
{
    delete ui;
}


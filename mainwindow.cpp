#include "mainwindow.h"
#include "databasemanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化并连接数据库
    if (DatabaseManager::instance().openDb()) {
        // 测试插入一条测试数据
        DatabaseManager::instance().insertData(25.5, 60.2, 500, "待机");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

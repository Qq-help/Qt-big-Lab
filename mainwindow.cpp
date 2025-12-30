#include "mainwindow.h"
#include "databasemanager.h"
#include "ui_mainwindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化并连接数据库
    if (DatabaseManager::instance().openDb()) {

    }

    // 创建模型，关联默认数据库
    m_model = new QSqlTableModel(this);
    m_model->setTable("sensor_data"); // 指定要显示的表名

    // 设置列名显示（把英文数据库字段变成中文表头）
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "时间");
    m_model->setHeaderData(2, Qt::Horizontal, "温度(℃)");
    m_model->setHeaderData(3, Qt::Horizontal, "湿度(%)");
    m_model->setHeaderData(4, Qt::Horizontal, "光照(Lux)");
    m_model->setHeaderData(5, Qt::Horizontal, "状态");

    // 加载数据
    m_model->select();

    // 将模型绑定到界面上的 TableView
    ui->tableView->setModel(m_model);

    // === 3. 美化表格显示 ===

    // 隐藏 ID 列（用户不需要看这个）
    ui->tableView->hideColumn(0);

    // 让列宽自动铺满窗口
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置整行选中（点一个格子选中一整行）
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 禁止编辑（只读模式）
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 隔行变色（更好看）
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

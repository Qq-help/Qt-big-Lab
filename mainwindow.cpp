#include "mainwindow.h"
#include "databasemanager.h"
#include "ui_mainwindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口最小宽度，保证表格内容不拥挤
    this->resize(550, 450);
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

    // === 3. 美化表格显示 (紧凑布局版) ===

    // 隐藏 ID 列
    ui->tableView->hideColumn(0);

    // 获取表头
    QHeaderView *header = ui->tableView->horizontalHeader();

    // 1. 时间列：固定宽度，刚好放下 "2025-12-30 16:00:00"
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(1, 150);

    // 2. 数据列（温度、湿度、光照）：给个小宽度，紧凑一点
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 80); // 温度

    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(3, 80); // 湿度

    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(4, 80); // 光照

    // 3. 状态列：自动拉伸，吃掉剩下的所有空间
    header->setSectionResizeMode(5, QHeaderView::Stretch);

    // 保持之前的设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

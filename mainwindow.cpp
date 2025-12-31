#include "mainwindow.h"
#include "databasemanager.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口最小宽度，保证表格内容不拥挤
    // --- Day 8 UI 美化：添加图标 ---
    // 使用 Qt 内置的“标准图标” (SP_...)，这样不用找图片也能有图标

    // 给“开启浇水”按钮加个“水滴/确认”图标
    ui->btnWater->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->btnWater->setIconSize(QSize(24, 24)); // 图标大一点

    // 给“开启通风”按钮加个“风扇/箭头”图标
    ui->btnFan->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->btnFan->setIconSize(QSize(24, 24));
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

    // --- Day 5 新增代码开始 ---

    // 1. 创建线程对象
    m_worker = new WorkerThread(this);

    // 2. 连接信号与槽：线程产生数据 -> 主窗口更新界面
    connect(m_worker, &WorkerThread::dataGenerated, this, &MainWindow::updateSensorData);

    // 3. 启动线程
    m_worker->start();

    // --- Day 5 新增代码结束 ---
    // --- Day 6 网络天气功能 ---
    m_weather = new WeatherManager(this);

    // 连接信号：当获取到天气时，更新 UI
    connect(m_weather, &WeatherManager::weatherUpdated, this, [=](double temp, QString desc){
        // 更新界面显示
        ui->lblCity->setText("北京市"); // 固定显示中文名
        ui->lblWeatherDesc->setText(desc); // 显示中文天气（如“晴”）
        ui->lblWeatherTemp->setText(QString::number(temp) + " °C");
    });

    // 启动时立即请求一次
    m_weather->fetchWeather("Beijing");

    // --- Day 7 自定义图表 ---
    m_chart = new ChartWidget(this);
    // 将自定义图表添加到 UI 中预留的布局里 (chartLayout)
    ui->chartLayout->addWidget(m_chart);
}

MainWindow::~MainWindow()
{
    // 停止并等待线程结束
    if (m_worker->isRunning()) {
        m_worker->stop();
        m_worker->quit();
        m_worker->wait();
    }
    delete ui;
}

void MainWindow::updateSensorData(double temp, double hum, int light, QString status)
{
    // 1. 更新 UI 顶部的实时数据显示 (使用你提供的 objectName)
    ui->labelTemp->setText(QString("温度: %1 °C").arg(temp, 0, 'f', 1)); // 保留1位小数
    ui->labelHum->setText(QString("湿度: %1 %").arg(hum, 0, 'f', 1));   // 保留1位小数

    // (可选) 如果你有光照 Label，也可以在这里更新，例如: ui->labelLight->setText(...)

    // 2. 写入数据库 (调用单例)
    DatabaseManager::instance().insertData(temp, hum, light, status);

    // 3. 刷新底部表格视图，显示最新插入的数据
    if (m_model) {
        m_model->select(); // 重新查询数据库

        // 4. 自动滚动到最后一行，方便查看最新状态
        ui->tableView->scrollToBottom();
    }
    if (m_chart) {
        m_chart->addData(temp);
    }
}

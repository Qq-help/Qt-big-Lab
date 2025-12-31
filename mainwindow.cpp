#include "mainwindow.h"
#include "databasemanager.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("邱晶晶智能农业监测系统 v1.0 (Final)");

    // --- UI 初始化部分 ---

    // 1. 设置按钮为“可选中”模式 (Toggle Button)
    ui->btnWater->setCheckable(true);
    ui->btnFan->setCheckable(true);

    // 设置图标 (保持原有代码)
    ui->btnWater->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->btnWater->setIconSize(QSize(24, 24));
    ui->btnFan->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->btnFan->setIconSize(QSize(24, 24));

    this->resize(600, 550); // 稍微调大一点窗口以便观察

    // 2. 数据库与模型初始化 (保持原有代码)
    if (DatabaseManager::instance().openDb()) {
        // ... (数据库连接成功日志)
    }

    m_model = new QSqlTableModel(this);
    m_model->setTable("sensor_data");
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "时间");
    m_model->setHeaderData(2, Qt::Horizontal, "温度(℃)");
    m_model->setHeaderData(3, Qt::Horizontal, "湿度(%)");
    m_model->setHeaderData(4, Qt::Horizontal, "光照(Lux)");
    m_model->setHeaderData(5, Qt::Horizontal, "状态");
    m_model->select();
    ui->tableView->setModel(m_model);

    // 表格列宽设置 (保持原有代码)
    ui->tableView->hideColumn(0);
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(1, 150);
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 80);
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(3, 80);
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(4, 80);
    header->setSectionResizeMode(5, QHeaderView::Stretch);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    // 3. 线程初始化
    m_worker = new WorkerThread(this);
    connect(m_worker, &WorkerThread::dataGenerated, this, &MainWindow::updateSensorData);
    m_worker->start();

    // 4. 天气模块 (保持原有代码)
    m_weather = new WeatherManager(this);
    connect(m_weather, &WeatherManager::weatherUpdated, this, [=](double temp, QString desc){
        ui->lblCity->setText("北京市");
        ui->lblWeatherDesc->setText(desc);
        ui->lblWeatherTemp->setText(QString::number(temp) + " °C");
    });
    m_weather->fetchWeather("Beijing");

    // 5. 图表模块 (保持原有代码)
    m_chart = new ChartWidget(this);
    ui->chartLayout->addWidget(m_chart);

    // --- 新增：手动关联信号槽 (如果 UI 文件中没有自动关联) ---
    // 注意：如果在 ui 设计器里右键“转到槽”生成了函数，这里不需要 connect
    // 下面通过 connect 显式连接，更安全
    connect(ui->btnWater, &QPushButton::toggled, this, &MainWindow::on_btnWater_toggled);
    connect(ui->btnFan, &QPushButton::toggled, this, &MainWindow::on_btnFan_toggled);
}

MainWindow::~MainWindow()
{
    if (m_worker->isRunning()) {
        m_worker->stop();
        m_worker->quit();
        m_worker->wait();
    }
    delete ui;
}

void MainWindow::updateSensorData(double temp, double hum, int light, QString status)
{
    // 更新 Label
    ui->labelTemp->setText(QString("温度: %1 °C").arg(temp, 0, 'f', 1));
    ui->labelHum->setText(QString("湿度: %1 %").arg(hum, 0, 'f', 1));

    // 写入数据库
    DatabaseManager::instance().insertData(temp, hum, light, status);

    // 刷新表格
    if (m_model) {
        m_model->select();
        ui->tableView->scrollToBottom();
    }

    // 更新曲线图
    if (m_chart) {
        m_chart->addData(temp, hum);
    }
}

void MainWindow::on_btnClear_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作",
                                  "确定要清空所有历史数据吗？\n此操作不可恢复！",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        if (query.exec("DELETE FROM sensor_data")) {
            m_model->select();
            QMessageBox::information(this, "成功", "历史记录已全部清空！");
        } else {
            QMessageBox::warning(this, "错误", "清空失败：" + query.lastError().text());
        }
    }
}

void MainWindow::on_btnWater_toggled(bool checked)
{
    // 1. 通知后台线程
    m_worker->setWateringStatus(checked);

    // 2. 仅更新文字，不要 setStyleSheet
    ui->btnWater->setText(checked ? "停止浇水" : "开启浇水");
}

void MainWindow::on_btnFan_toggled(bool checked)
{
    m_worker->setFanStatus(checked);
    ui->btnFan->setText(checked ? "关闭通风" : "开启通风");
}

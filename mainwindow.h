#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "workerthread.h"
#include "weathermanager.h"
#include "chartwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // 处理传感器数据更新
    void updateSensorData(double temp, double hum, int light, QString status);

private slots:
    // 清空数据按钮
    void on_btnClear_clicked();

    // --- 新增：远程控制按钮槽函数 ---
    void on_btnWater_toggled(bool checked); // 浇水按钮切换
    void on_btnFan_toggled(bool checked);   // 通风按钮切换

private:
    Ui::MainWindow *ui;
    QSqlTableModel *m_model;   // 数据库模型
    WorkerThread *m_worker;    // 后台采集线程
    WeatherManager *m_weather; // 天气管理
    ChartWidget *m_chart;      // 自定义图表
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QFile>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置全局样式表
    QString qss = R"(
        /* 全局背景色：浅灰白，看起来干净 */
        QMainWindow {
            background-color: #F0F2F5;
        }

        /* 所有的标签（文字） */
        QLabel {
            font-family: "Microsoft YaHei";
            color: #333333;
        }

        /* 大标题特有样式（通过 objectName 定位） */
        QLabel#labelTitle {
            color: #2c3e50;
            font-weight: bold;
            padding: 10px;
        }

        /* 两个分组框（环境监测、远程控制） */
        QGroupBox {
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            margin-top: 20px; /* 给标题留位置 */
            font-weight: bold;
            color: #555;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 10px;
            background-color: white; /* 标题背景遮挡边框 */
        }

/* 按钮样式：清新农场绿 (渐变) */
        QPushButton {
            /* 从深绿到嫩绿的渐变 */
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #56ab2f, stop:1 #a8e063);
            border: none;
            border-radius: 15px; /* 圆角 */
            color: white;
            font-weight: bold;
            padding: 8px 16px;
            min-width: 80px;
        }

        /* 按钮鼠标悬停时 (可选，增加互动感) */
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #66bb3f, stop:1 #b8f073);
        }

/* 按钮按下时：变为深绿色，模拟按压感 */
        QPushButton:pressed {
            background-color: #2e7d32; /* 深森林绿 */
            padding-top: 10px;         /* 文字下沉 */
        }

        /* 按钮被选中（开启）时的状态：保持深绿色，边框加深 */
        QPushButton:checked {
            background-color: #2e7d32; /* 保持深绿色 */
            border: 2px solid #1b5e20; /* 墨绿色边框，增加立体感 */
        }

        /* 表格样式 */
        QTableView {
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            gridline-color: #f0f0f0;
            selection-background-color: #a1c4fd;
        }
        QHeaderView::section {
            background-color: #f8f9fa;
            border: none;
            border-bottom: 1px solid #e0e0e0;
            padding: 5px;
            font-weight: bold;
        }
 /* --- Day 6 新增：天气区域专属样式 --- */

        /* 城市名称：使用低调的灰色 */
        QLabel#lblCity {
            color: #7f8c8d;
            font-size: 13px;
            font-weight: normal;
        }

        /* 天气状况（如晴/雨）：使用你按钮同款的深绿色 */
        QLabel#lblWeatherDesc {
            color: #56ab2f;
            font-size: 15px;
            font-weight: bold;
        }

        /* 室外气温：使用橙色，与室内蓝色的区分开 */
        QLabel#lblWeatherTemp {
            color: #e67e22;
            font-size: 18px;
            font-weight: bold;
            font-family: "Arial";
        }
    )";
    a.setStyleSheet(qss);

    MainWindow w;
    w.show();
    return a.exec();
}

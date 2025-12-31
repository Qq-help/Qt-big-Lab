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

/* --- 按钮基础样式 --- */
QPushButton {
    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #56ab2f, stop:1 #a8e063);
    border: none;
    border-radius: 15px;
    color: white;
    font-weight: bold;
    padding: 8px 16px;
    min-width: 80px;
}

/* 鼠标悬停：稍微变亮一点 */
QPushButton:hover {
    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #66bb3f, stop:1 #b8f073);
}

/* --- 关键修改：按下 (pressed) 和 选中状态 (checked) 保持一致 --- */
/* 我们使用更重的绿色：#388E3C 是标准的深绿色 */
QPushButton:pressed, QPushButton:checked {
    background-color: #388E3C;
    /* 如果想要更明显的按压感，可以保留这个下沉效果 */
    padding-top: 10px;
    border: 2px solid #2E7D32; /* 边框再深一点 */
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
            color: black;
            font-size: 16px;
            font-weight: normal;
        }

        /* 天气状况（如晴/雨）：使用你按钮同款的深绿色 */
        QLabel#lblWeatherDesc {
            color: black;
            font-size: 15px;
            font-weight: bold;
        }

        /* 室外气温：使用橙色，与室内蓝色的区分开 */
        QLabel#lblWeatherTemp {
            color: black;
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

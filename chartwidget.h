#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPointF>

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);

    // 添加数据点，并触发界面重绘
    void addData(double temp, double hum);

protected:
    // 处理自定义绘制逻辑
    void paintEvent(QPaintEvent *event) override;

private:
    // 最大保存的数据点数，用于控制 X 轴滚动
    // 根据 .cpp 中的 xStep 计算逻辑，通常设为固定值，例如 20 或 30
    const int MAX_POINTS = 20;

    // 存储历史数据的容器
    QList<double> m_tempData;
    QList<double> m_humData;
};

#endif // CHARTWIDGET_H

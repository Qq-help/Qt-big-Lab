#include "chartwidget.h"
#include <QPainter>
#include <QPen>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget{parent}
{
    // 设置背景色为白色
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void ChartWidget::addData(double value)
{
    m_data.append(value);
    // 如果数据太多，移除最早的，保持图表在移动
    if (m_data.size() > MAX_POINTS) {
        m_data.removeFirst();
    }
    // 触发重绘（告诉界面：该刷新了）
    update();
}

void ChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿，线条更平滑

    int w = width();
    int h = height();
    int padding = 40; // 边距

    // 1. 画坐标轴
    painter.setPen(QPen(Qt::gray, 2));
    painter.drawLine(padding, h - padding, w - padding, h - padding); // X轴
    painter.drawLine(padding, padding, padding, h - padding);         // Y轴

    // 2. 画文字
    painter.drawText(padding - 30, padding, "35°C");      // Y轴顶端
    painter.drawText(padding - 30, h - padding, "15°C");  // Y轴底端

    // 如果没有数据，直接返回
    if (m_data.isEmpty()) return;

    // 3. 画折线
    painter.setPen(QPen(QColor("#56ab2f"), 3)); // 漂亮的绿色线条

    // 计算每个点的间距
    double xStep = (double)(w - 2 * padding) / (MAX_POINTS - 1);

    // 简单的映射：假设温度范围是 15度 ~ 35度
    // 实际项目中可以动态计算最大最小值
    double minTemp = 15.0;
    double maxTemp = 35.0;
    double range = maxTemp - minTemp;

    QPointF lastPoint;

    for (int i = 0; i < m_data.size(); ++i) {
        double val = m_data[i];

        // 计算 X 坐标
        double x = padding + i * xStep;

        // 计算 Y 坐标 (注意：屏幕坐标系 Y 轴是向下的，所以要用 h - padding 减去高度)
        double ratio = (val - minTemp) / range;
        // 限制一下范围，防止画出界
        if(ratio < 0) ratio = 0;
        if(ratio > 1) ratio = 1;

        double y = (h - padding) - ratio * (h - 2 * padding);

        QPointF currentPoint(x, y);

        if (i > 0) {
            painter.drawLine(lastPoint, currentPoint);
        }
        lastPoint = currentPoint;

        // 画个小圆点
        painter.setBrush(Qt::white);
        painter.drawEllipse(currentPoint, 4, 4);
    }
}

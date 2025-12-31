#include "chartwidget.h"
#include <QPainter>
#include <QPen>
ChartWidget::ChartWidget(QWidget *parent)
    : QWidget{parent}
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
void ChartWidget::addData(double temp, double hum)
{
    // 1. 存温度
    m_tempData.append(temp);
    if (m_tempData.size() > MAX_POINTS) m_tempData.removeFirst();
 // 2. 存湿度
    m_humData.append(hum);
    if (m_humData.size() > MAX_POINTS) m_humData.removeFirst();

    update();
}
void ChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0) return;

    int padding = 35; // 稍微增加一点边距，给文字留位置

    // --- 1. 画背景网格和刻度 (0, 20, 40, 60, 80, 100) ---
    double minVal = 0.0;
    double maxVal = 100.0;
    double range = maxVal - minVal;

    // 设定画笔：灰色虚线用于网格
    QPen gridPen(QColor(220, 220, 220), 1, Qt::DashLine);
    QPen textPen(Qt::black);

    // 循环画 6 条线：0, 20, 40, 60, 80, 100
    for (int i = 0; i <= 5; ++i) {
        double val = i * 20; // 0, 20, 40...
        double ratio = (val - minVal) / range;
        double y = (h - padding) - (ratio * (h - 2 * padding));

        // 画网格线 (虚线)
        painter.setPen(gridPen);
        painter.drawLine(padding, y, w - padding, y);

        // 画刻度文字
        painter.setPen(textPen);
        // 稍微调整一下文字位置，让它垂直居中于线
        QRect textRect(0, y - 10, padding - 5, 20);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
    }

    // --- 2. 画坐标轴实体线 ---
    painter.setPen(QPen(Qt::gray, 2));
    painter.drawLine(padding, padding, padding, h - padding);         // Y轴
    painter.drawLine(padding, h - padding, w - padding, h - padding); // X轴

    // 在左上角标上单位
    painter.drawText(padding - 10, padding - 10, "单位: °C / %");

    // --- 3. 绘图辅助函数 (Lambda) ---
    auto drawLine = [&](const QList<double>& data, QColor color) {
        if (data.isEmpty()) return;

        painter.setPen(QPen(color, 3)); // 线条加粗一点到 3

        double xStep = (double)(w - 2 * padding) / (MAX_POINTS - 1);
        int startOffset = MAX_POINTS - data.size();

        QPointF lastPoint;
        for (int i = 0; i < data.size(); ++i) {
            double val = data[i];
            if (val > maxVal) val = maxVal;
            if (val < minVal) val = minVal;

            double x = padding + (startOffset + i) * xStep;
            double ratio = (val - minVal) / range;
            double y = (h - padding) - (ratio * (h - 2 * padding));

            QPointF currentPoint(x, y);
            if (i > 0) painter.drawLine(lastPoint, currentPoint);
            lastPoint = currentPoint;

            // 画个白色填充的小圆点，更精致
            painter.save();
            painter.setBrush(Qt::white);
            painter.setPen(QPen(color, 2));
            painter.drawEllipse(currentPoint, 3, 3);
            painter.restore();
        }
    };

    // --- 4. 画曲线 ---
    drawLine(m_tempData, QColor("#2e7d32")); // 绿色温度
    drawLine(m_humData, QColor("#2980b9"));  // 蓝色湿度

    // --- 5. 画图例 ---
    // 画在右上角
    int legendX = w - 100;
    int legendY = padding;

    // 温度图例
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#2e7d32"));
    painter.drawEllipse(legendX, legendY, 8, 8);
    painter.setPen(Qt::black);
    painter.drawText(legendX + 15, legendY + 8, "温度");

    // 湿度图例
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#2980b9"));
    painter.drawEllipse(legendX, legendY + 20, 8, 8);
    painter.setPen(Qt::black);
    painter.drawText(legendX + 15, legendY + 28, "湿度");
}

#include "chartwidget.h"
#include <QPainter>
#include <QPen>
#include <QResizeEvent>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget{parent}
{
    // 1. 设置背景为白色
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);

    // 2. 强制策略：尽可能占用空间
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ChartWidget::addData(double value)
{
    m_data.append(value);
    // 保持数据量在 MAX_POINTS 以内
    if (m_data.size() > MAX_POINTS) {
        m_data.removeFirst();
    }
    // 触发重绘
    update();
}

void ChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 获取当前控件的真实宽高
    int w = width();
    int h = height();

    // 如果窗口太小，就不画了，防止除以0报错
    if (w <= 0 || h <= 0) return;

    int padding = 30; // 边距

    // --- 1. 画坐标轴 ---
    painter.setPen(QPen(Qt::gray, 1));
    // Y轴
    painter.drawLine(padding, padding, padding, h - padding);
    // X轴
    painter.drawLine(padding, h - padding, w - padding, h - padding);

    // --- 2. 画刻度文字 ---
    painter.setPen(Qt::black);
    painter.drawText(5, padding + 5, "35°C");       // 最高温
    painter.drawText(5, h - padding + 5, "15°C");   // 最低温

    // --- 3. 画折线 (核心逻辑) ---
    if (m_data.isEmpty()) return;

    painter.setPen(QPen(QColor("#56ab2f"), 3)); // 绿色线条，宽度3

    // 设定温度范围 (固定 15~35 度，超出这个范围会被截断)
    double minTemp = 15.0;
    double maxTemp = 35.0;
    double tempRange = maxTemp - minTemp;

    // 计算 X 轴每个点的间距
    // 即使数据不够 20 个，我们也按 20 个的宽度来算，这样数据会靠左显示，或者我们可以让它靠右
    // 这里采用：固定间距，数据向左流动
    double xStep = (double)(w - 2 * padding) / (MAX_POINTS - 1);

    QPointF lastPoint;

    // 这里的逻辑是：让最新的数据永远在最右边
    // startOffset 用于计算当数据不足 20 个时，从哪里开始画
    int startOffset = MAX_POINTS - m_data.size();

    for (int i = 0; i < m_data.size(); ++i) {
        double val = m_data[i];

        // 限制数值范围，防止画出界
        if (val > maxTemp) val = maxTemp;
        if (val < minTemp) val = minTemp;

        // 计算 X: 加上偏移量，确保最新的点在最右侧
        double x = padding + (startOffset + i) * xStep;

        // 计算 Y: 注意屏幕坐标系 Y 是向下的
        double ratio = (val - minTemp) / tempRange;
        double y = (h - padding) - (ratio * (h - 2 * padding));

        QPointF currentPoint(x, y);

        if (i > 0) {
            painter.drawLine(lastPoint, currentPoint);
        }
        lastPoint = currentPoint;

        // 画数据点
        painter.setBrush(Qt::white);
        painter.drawEllipse(currentPoint, 3, 3);
    }
}

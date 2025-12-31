#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QList>

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);

    // 添加数据的方法
    void addData(double value);

protected:
    // 重写绘图事件（核心）
    void paintEvent(QPaintEvent *event) override;

private:
    QList<double> m_data; // 存储最近的温度数据
    const int MAX_POINTS = 20; // 最多显示多少个点
};

#endif // CHARTWIDGET_H

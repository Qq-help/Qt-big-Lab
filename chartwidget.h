#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QList>

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);

    // 修改：同时接收温度和湿度
    void addData(double temp, double hum);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<double> m_tempData; // 温度数据
    QList<double> m_humData;  // 湿度数据
    const int MAX_POINTS = 20;
};

#endif

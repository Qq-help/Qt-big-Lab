#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager> // 网络访问管理器
#include <QNetworkReply>         // 网络响应

class WeatherManager : public QObject
{
    Q_OBJECT
public:
    explicit WeatherManager(QObject *parent = nullptr);

    // 主动调用的函数：开始获取天气
    void fetchWeather(const QString &city);

signals:
    // 信号：当获取到天气数据时，发送给主界面
    // 参数：温度(double), 天气描述(String)
    void weatherUpdated(double temp, QString description);

private slots:
    // 内部槽函数：处理网络返回的数据
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager; // 负责发快递的“邮递员”
};

#endif // WEATHERMANAGER_H

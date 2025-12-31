#include "weathermanager.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QUrl>

WeatherManager::WeatherManager(QObject *parent)
    : QObject{parent}
{
    m_manager = new QNetworkAccessManager(this);

    // 连接信号槽：请求结束 -> 处理结果
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &WeatherManager::onReplyFinished);
}

void WeatherManager::fetchWeather(const QString &city)
{
    // 1. 准备 API 地址
    // 这里我们使用 Open-Meteo 的免费接口
    // latitude=39.90, longitude=116.40 是北京的坐标
    // current_weather=true 表示只获取当前天气
    QString urlStr = "https://api.open-meteo.com/v1/forecast?"
                     "latitude=39.90&longitude=116.40&current_weather=true";

    qDebug() << "正在请求天气 URL:" << urlStr;

    // 2. 创建请求对象
    QNetworkRequest request;
    request.setUrl(QUrl(urlStr));

    // 3. 发送 GET 请求
    m_manager->get(request);
}

void WeatherManager::onReplyFinished(QNetworkReply *reply)
{
    // 1. 检查是否有错误
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "网络错误:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    // 2. 读取服务器返回的所有数据
    QByteArray jsonData = reply->readAll();
    qDebug() << "收到服务器数据:" << jsonData;

    // (下一阶段我们将在这里解析 JSON)

    // 3. 清理内存
    reply->deleteLater();
}

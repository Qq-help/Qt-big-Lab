#include "weathermanager.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

WeatherManager::WeatherManager(QObject *parent)
    : QObject{parent}
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &WeatherManager::onReplyFinished);
}

void WeatherManager::fetchWeather(const QString &city)
{
    // 北京坐标: 39.90, 116.40
    QString urlStr = "https://api.open-meteo.com/v1/forecast?"
                     "latitude=39.90&longitude=116.40&current_weather=true";

    QNetworkRequest request;
    request.setUrl(QUrl(urlStr));
    m_manager->get(request);
}

void WeatherManager::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "网络错误:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    // 1. 获取原始数据
    QByteArray jsonData = reply->readAll();

    // 2. 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "JSON 解析失败";
        reply->deleteLater();
        return;
    }

    QJsonObject root = doc.object();

    // 提取 "current_weather" 对象
    if (root.contains("current_weather")) {
        QJsonObject current = root["current_weather"].toObject();

        // 提取温度
        double temp = current["temperature"].toDouble();

        // 提取天气代码 (WMO Code) 并转为中文
        int code = current["weathercode"].toInt();
        QString weatherText = "未知";

        // 简单的 WMO 代码翻译表
        if (code == 0) weatherText = "晴";
        else if (code >= 1 && code <= 3) weatherText = "多云";
        else if (code >= 45 && code <= 48) weatherText = "雾";
        else if (code >= 51 && code <= 55) weatherText = "毛毛雨";
        else if (code >= 61 && code <= 65) weatherText = "雨";
        else if (code >= 71 && code <= 77) weatherText = "雪";
        else if (code >= 80 && code <= 82) weatherText = "阵雨";
        else if (code >= 95) weatherText = "雷雨";

        // 3. 发送信号给主界面
        emit weatherUpdated(temp, weatherText);

        qDebug() << "解析成功: 北京" << temp << "度" << weatherText;
    }

    reply->deleteLater();
}

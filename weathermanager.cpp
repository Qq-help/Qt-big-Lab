#include "weathermanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

WeatherManager::WeatherManager(QObject *parent)
    : QObject{parent}
{
    // 1. 创建网络管理器
    m_manager = new QNetworkAccessManager(this);

    // 2. 连接信号：当请求完成时，调用 onReplyFinished
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &WeatherManager::onReplyFinished);
}

void WeatherManager::fetchWeather(const QString &city)
{
    // 暂时打印一下，证明函数被调用了
    qDebug() << "正在尝试获取天气，城市：" << city;

    // 下一阶段我们将在这里写真正的联网代码
}

void WeatherManager::onReplyFinished(QNetworkReply *reply)
{
    // 暂时留空，下一阶段处理数据
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "网络请求成功！";
    } else {
        qDebug() << "网络请求失败：" << reply->errorString();
    }

    // 务必手动删除 reply 对象，防止内存泄漏
    reply->deleteLater();
}

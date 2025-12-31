#include "workerthread.h"
#include <QRandomGenerator>

WorkerThread::WorkerThread(QObject *parent)
    : QThread{parent}, m_running(true)
{
}

void WorkerThread::stop()
{
    m_running = false;
}

void WorkerThread::run()
{
    while (m_running) {
        // 1. 模拟生成随机数据 (修复了小数生成的写法)

        // 温度：20.0 ~ 35.0
        // generateDouble() 生成 0~1 之间的小数
        double temp = 20.0 + (QRandomGenerator::global()->generateDouble() * 15.0);

        // 湿度：30.0 ~ 80.0
        double hum = 30.0 + (QRandomGenerator::global()->generateDouble() * 50.0);

        // 光照：0 ~ 1000 (整数可以直接用 bounded)
        int light = QRandomGenerator::global()->bounded(0, 1001);

        // 2. 自动控制逻辑
        QString status = "正常";
        if (hum < 40.0) {
            status = "自动浇水";
        }

        // 3. 发送信号
        emit dataGenerated(temp, hum, light, status);

        // 4. 休眠 2 秒
        msleep(2000);
    }
}

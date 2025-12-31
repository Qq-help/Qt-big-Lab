#include "workerthread.h"
#include <QRandomGenerator>
#include <QDebug>

WorkerThread::WorkerThread(QObject *parent)
    : QThread{parent}
    , m_running(true)
    , m_isWatering(false)
    , m_isFanOn(false)
    , m_currentTemp(25.0) // 初始温度
    , m_currentHum(50.0)  // 初始湿度
{
}

void WorkerThread::stop()
{
    m_running = false;
}

// --- 新增：设置状态 ---
void WorkerThread::setWateringStatus(bool on)
{
    m_isWatering = on;
}

void WorkerThread::setFanStatus(bool on)
{
    m_isFanOn = on;
}

void WorkerThread::run()
{
    while (m_running) {
        // === 1. 模拟环境数据变化逻辑 ===

        // --- 温度逻辑 ---
        if (m_isFanOn) {
            // 如果开了风扇，温度逐渐下降，最低降到 15度
            m_currentTemp -= 0.5;
            if (m_currentTemp < 15.0) m_currentTemp = 15.0;
        } else {
            // 自然回升/波动 (模拟白天升温)
            m_currentTemp += (QRandomGenerator::global()->generateDouble() - 0.3);
        }

        // --- 湿度逻辑 ---
        if (m_isWatering) {
            // 如果正在浇水，湿度快速上升，最高 95%
            m_currentHum += 2.0;
            if (m_currentHum > 95.0) m_currentHum = 95.0;
        } else {
            // 自然蒸发，湿度缓慢下降
            m_currentHum -= 0.5;
            if (m_currentHum < 20.0) m_currentHum = 20.0;
        }

        // 添加一点随机抖动，让曲线看起来更真实
        double finalTemp = m_currentTemp + (QRandomGenerator::global()->generateDouble() * 0.4 - 0.2);
        double finalHum = m_currentHum + (QRandomGenerator::global()->generateDouble() * 1.0 - 0.5);

        // 光照：0 ~ 1000
        int light = QRandomGenerator::global()->bounded(0, 1001);

        // === 2. 确定设备状态字符串 ===
        QString status = "正常监测";

        // 优先显示手动控制的状态
        if (m_isWatering && m_isFanOn) {
            status = "浇水&通风中";
        } else if (m_isWatering) {
            status = "手动浇水";
        } else if (m_isFanOn) {
            status = "手动通风";
        } else {
            // 自动逻辑保留
            if (finalHum < 30.0) status = "缺水预警";
        }

        // === 3. 发送信号 ===
        emit dataGenerated(finalTemp, finalHum, light, status);

        // === 4. 休眠 2 秒 ===
        msleep(2000);
    }
}

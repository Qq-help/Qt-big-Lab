#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QObject>
#include <atomic> // 用于线程安全的布尔标志

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);

    // 控制线程停止
    void stop();

    // --- 新增：外部控制接口 ---
    void setWateringStatus(bool on); // 设置浇水状态
    void setFanStatus(bool on);      // 设置风扇状态

signals:
    // 发送数据信号
    void dataGenerated(double temp, double hum, int light, QString status);

protected:
    void run() override;

private:
    std::atomic<bool> m_running;

    // --- 新增：控制标志位 (使用 atomic 保证线程安全) ---
    std::atomic<bool> m_isWatering; // 是否正在浇水
    std::atomic<bool> m_isFanOn;    // 是否开启通风

    // --- 新增：模拟环境的当前值 (为了实现连续变化) ---
    double m_currentTemp;
    double m_currentHum;
};

#endif // WORKERTHREAD_H

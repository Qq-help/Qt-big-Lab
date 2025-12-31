#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QObject>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);
    void stop(); // 用于安全停止线程

protected:
    void run() override; // 线程的主逻辑入口

signals:
    // 信号：将生成的数据传递给主界面
    void dataGenerated(double temp, double hum, int light, QString status);

private:
    bool m_running; // 线程运行标志位
};

#endif // WORKERTHREAD_H

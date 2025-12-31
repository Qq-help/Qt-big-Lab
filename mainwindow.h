#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlTableModel>
#include <QMainWindow>
#include "workerthread.h"
#include "weathermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSensorData(double temp, double hum, int light, QString status);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *m_model;
    WorkerThread *m_worker;
    WeatherManager *m_weather;

};
#endif // MAINWINDOW_H

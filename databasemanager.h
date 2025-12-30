#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

// 数据库管理类（单例模式，保证全局只有一个数据库连接）
class DatabaseManager
{
public:
    // 获取单例实例
    static DatabaseManager& instance();

    // 连接数据库
    bool openDb();

    // 插入一条传感器数据
    bool insertData(double temp, double humi, int light, const QString& status);

    // 关闭数据库
    void closeDb();

private:
    // 私有构造函数，禁止外部直接创建对象
    DatabaseManager();
    ~DatabaseManager();

    QSqlDatabase m_db; // 数据库对象
};

#endif // DATABASEMANAGER_H

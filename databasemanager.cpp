#include "databasemanager.h"
#include <QCoreApplication>
#include <QDir>

// 获取单例实例
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
    // 构造函数中不做复杂操作
}

DatabaseManager::~DatabaseManager()
{
    closeDb();
}

bool DatabaseManager::openDb()
{
    // 1. 添加 SQLite 数据库驱动
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    // 2. 设置数据库文件路径 (存放在运行目录下)
    QString dbPath = QCoreApplication::applicationDirPath() + "/agri_data.db";
    m_db.setDatabaseName(dbPath);

    qDebug() << "Database path:" << dbPath;

    // 3. 打开数据库
    if (!m_db.open()) {
        qDebug() << "Error: Failed to open database." << m_db.lastError();
        return false;
    }

    // 4. 如果表不存在，创建表
    QSqlQuery query;
    // SQL语句：创建一个叫 sensor_data 的表
    QString sql = "CREATE TABLE IF NOT EXISTS sensor_data ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "timestamp DATETIME, "
                  "temperature REAL, "
                  "humidity REAL, "
                  "light_lux INTEGER, "
                  "device_status TEXT)";

    if (!query.exec(sql)) {
        qDebug() << "Error: Failed to create table." << query.lastError();
        return false;
    }

    qDebug() << "Database initialized successfully!";
    return true;
}

bool DatabaseManager::insertData(double temp, double humi, int light, const QString& status)
{
    QSqlQuery query;
    // 准备 SQL 插入语句
    query.prepare("INSERT INTO sensor_data (timestamp, temperature, humidity, light_lux, device_status) "
                  "VALUES (:time, :temp, :humi, :light, :status)");

    // 绑定数据 (使用当前时间)
    query.bindValue(":time", QDateTime::currentDateTime());
    query.bindValue(":temp", temp);
    query.bindValue(":humi", humi);
    query.bindValue(":light", light);
    query.bindValue(":status", status);

    // 执行插入
    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError();
        return false;
    }
    return true;
}

void DatabaseManager::closeDb()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

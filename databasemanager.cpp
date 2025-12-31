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

bool DatabaseManager::insertData(double temp, double hum, int light, const QString &status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO sensor_data (timestamp, temperature, humidity, light_lux, device_status) "
                  "VALUES (:time, :temp, :hum, :light, :status)");

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":time", currentTime);
    query.bindValue(":temp", temp);
    query.bindValue(":hum", hum);
    query.bindValue(":light", light);
    query.bindValue(":status", status);

    bool success = query.exec();

    if (success) {
        // --- Day 8 新增：自动清理旧数据 ---
        // 只保留 ID 最大的 100 条记录（即最新的 100 条）
        // 这里的逻辑是：删除那些 "不在（最近100条ID列表）" 里面的所有记录
        QSqlQuery cleanQuery;
        cleanQuery.exec("DELETE FROM sensor_data WHERE id NOT IN "
                        "(SELECT id FROM sensor_data ORDER BY id DESC LIMIT 100)");
        // -------------------------------
    } else {
        qDebug() << "插入数据失败:" << query.lastError().text();
    }

    return success;
}

void DatabaseManager::closeDb()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

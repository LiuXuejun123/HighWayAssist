#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
class Calibration_MCU
{
public:
    QString Name;
    QString Module;
    QString DataType;
    QString Value;
    QString demRow;
    QString demCol;
    QString VehType;
};
class DataBaseManager
{
public:
    DataBaseManager(const QString& SqlName):dbName(SqlName){};
    bool SQL_Init();
    void SQL_Create();
    void SQL_Close();
    void SQL_Update(const QString& CalibartionName,const QString& Value);
    void SQL_Insert(Calibration_MCU Calibartion);
    Calibration_MCU SQL_Search(const QString& CalibartionName);

private:
    QString dbName;
    QSqlDatabase db;
};

#endif // MYSQLITE_H

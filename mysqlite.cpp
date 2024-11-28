#include "mysqlite.h"

bool DataBaseManager::SQL_Init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbName);
    if(!db.open())
    {
        qDebug()<<"Error:Unable to open database";
        qDebug()<<db.lastError().text();
        return false;
    }
    return true;
}
void DataBaseManager::SQL_Close()
{
    this->db.close();
}
void DataBaseManager::SQL_Create()
{
    QSqlQuery query;
    QString CreateMainTable_Query = "CREATE TABLE IF NOT EXISTS Calibration ("
                                    "calname TEXT PRIMARY KEY,"
                                    "moulde TEXT,"
                                    "datatype TEXT,"
                                    "value TEXT,"
                                    "demrow INTEGER,"
                                    "demcol INTEGER)";
    if(!query.exec(CreateMainTable_Query))
    {
        qDebug()<<"ERROR:  File Create Table" <<query.lastError().text();
    }else
    {
        qDebug()<<"SUCCESS: Create Table" ;
    }

    QString CreateHistoryTable_Query =  "CREATE TABLE IF NOT EXISTS CalibrationChangeList ("
                                       "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "calname TEXT ,"
                                       "oldvalue TEXT,"
                                       "newvalue TEXT,"
                                       "modifytime DATETIME)";
    if(!query.exec(CreateHistoryTable_Query))
    {
        qDebug()<<"ERROR:  File Create Table" <<query.lastError().text();
    }else
    {
        qDebug()<<"SUCCESS: Create Table" ;
    }
    QString CreateTriggerEvent_Query = R"(CREATE TRIGGER IF NOT EXISTS update_calibration_trigger
                                       AFTER UPDATE ON Calibration
                                       FOR EACH ROW
                                       BEGIN
                                           INSERT INTO CalibrationChangeList (calname, oldvalue, newvalue, modifytime)
                                           VALUES (OLD.calname, OLD.value, NEW.value, DATETIME('now'));
                                       END;)";
    if(!query.exec(CreateTriggerEvent_Query))
    {
        qDebug()<<"ERROR:  Create Table Event" <<query.lastError().text();
    }else
    {
        qDebug()<<"SUCCESS: Create Table Event" ;
    }

}


void DataBaseManager::SQL_Insert(Calibration_MCU Calibartion)
{   QSqlQuery query;
    query.prepare("INSERT INTO Calibration (calname, moulde,datatype,value,demrow,demcol) VALUES (:calname,:moulde,:datatype,:value,:demrow,:demcol)");
    query.bindValue(":calname",Calibartion.Name);
    query.bindValue(":moulde",Calibartion.Module);
    query.bindValue(":datatype",Calibartion.DataType);
    query.bindValue(":value",Calibartion.Value);
    query.bindValue(":demrow",Calibartion.demRow);
    query.bindValue(":demcol",Calibartion.demCol);
    if(!query.exec())
    {
        qDebug()<<"ERROR: Fail insert data"<<Calibartion.Name;
        return;
    }
    qDebug()<<"SUCCESS: insert data";
    return;
}

Calibration_MCU DataBaseManager::SQL_Search(const QString& CalibartionName)
{
    Calibration_MCU Result;

    QSqlQuery query;
    query.prepare("SELECT * FROM Calibration WHERE calname = :calname");
    query.bindValue(" :calname",CalibartionName);
    if(!query.exec())
    {
        qDebug()<<"ERROR: Fail Search Data";
        return Result;
    }
    if (query.next())
    {
        //calname, moulde,datatype,value,demrow,demcol
        Result.Name = query.value("calname").toString();
        Result.Module = query.value("moulde").toString();
        Result.DataType = query.value("datatype").toString();
        Result.Value = query.value("value").toString();
        Result.demRow = query.value("demrow").toString();
        Result.demCol = query.value("demcol").toString();
        return Result;
    }else
    {
        qDebug()<<"ERROR: Fail Search Data";
        return Result;
    }

}
void DataBaseManager::SQL_Update(const QString& CalibartionName,const QString& Value)
{
    QSqlQuery query;
    query.prepare("UPDATE Calibration SET value = :value WHERE calname = :calname");
    query.bindValue(":value",Value);
    query.bindValue(":calname",CalibartionName);
    if(!query.exec())
    {
        qDebug()<<"ERROR: SQL Update Fail";
        return;
    }
    return;
}


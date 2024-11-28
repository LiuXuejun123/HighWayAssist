#ifndef GENERAL_H
#define GENERAL_H
#include <QString>
#include <QFileInfo>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QSize>
#include <QProcess>
#include <QDateTime>
#include "CustomDataType.h"
class General
{
public:
    General();
    static QString SearchFileCSV();
    static QString SearchFileXlsx();
    static QString SearchFileDBGRecbin();
    static QString SearchFileCanRecbin();
    static QString SearchFileDBC();
    static QString SearchFileSQL();
    static QVector<QStringList> ReadCsvFile(QString FileName);
    static QStringList GetCsvHeader(QVector<QStringList> Data);

    static QString FileRename(QString OriginStr,QString NewName);
    static QString GetFileRecordTime(QString OriginStr);
    static QString GetFileFolder(const QString& OriginStr);
    static QString CreateSaveFileFolder(const QString& OriginStr,const QString& FolderName);
    static bool areSameDayandTime(const QString& fileName1,const QString& fileName2);
    static bool isTimeDifferenceWithIn30Second(const QString& fileName1,const QString& fileName2);



    static void DBGRecbin2Mat(const QString& DBGFile1,const QString& DBGFile2,const QString& SaveFolderPath,const QString& Platform);
    static void CanRecbin2Mat(const QString& CanFile1,const QString& CanFile2,const QString& DBCFile,const QString& LogSignal,const QString& SaveFolderPath,const QString& Platform);
    static void CreateIssueDescriptionFile(const QString& folderPath,const QString& fileName,const QString& Content);

    static QString EnumLanePosition2LaneType(E_LanePosition Pos);
    static QString EnumPlatform2SysPlatform(E_Platform Platform);
    static QString EnumLanePosition2EdgeType(E_LanePosition Pos);
};
double ploy(double x,const QVector<double>& Coeff);
double myabs(double x);
double myarctan(double x);
double mycos(double x);
double mysin(double x);
#endif // GENERAL_H

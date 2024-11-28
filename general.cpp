#include "general.h"
#include <QtMath>
General::General(){}
/*Search File*/
QString General::SearchFileCSV()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open CSV File","","CSV Files (*.csv)");
    return filename;
}
QString General::SearchFileXlsx()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open Xlsx File","","Xlsx Files (*.xlsx)");
    return filename;
}
QString General::SearchFileCanRecbin()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open Can recbin File","","Recbin Files (*can_CHASIS.recbin)");
    return filename;
}

QString General::SearchFileDBGRecbin()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open DBG recbin File","","Recbin Files (*dbg.recbin)");
    return filename;
}
QString General::SearchFileDBC()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open dbc File","","DBC Files (*.dbc)");
    return filename;
}
QString General::SearchFileSQL()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open DataBase File","","SQL Files (*.db)");
    return filename;
}


/*CSV Process*/
QVector<QStringList> General::ReadCsvFile(QString FileName)
{
    QVector<QStringList> Data;
    QFile file(FileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug() << "File open Failed" <<file.errorString();
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields  = line.split(',');
        Data.push_back(fields);
    }
    file.close();
    return Data;
}
QStringList General::GetCsvHeader(QVector<QStringList> Data)
{
    return Data[0];
}
//File Name Process
QString General::FileRename(QString OriginStr,QString NewName)
{
    QString  SubStr = OriginStr.mid(0,OriginStr.lastIndexOf("dbg"));
    QString Newstr = SubStr + "dbg" + "_FusionOutput_" + NewName +".csv";
    return Newstr;
}
QString General::GetFileRecordTime(QString OriginStr)
{
    QRegularExpression regex(R"(_(\d{8})_(\d{6})_)");
    QRegularExpressionMatch match = regex.match(OriginStr);
    QString dateString = "000000";
    QString timeString = "000000";
    if(match.hasMatch())
    {
        qDebug()<<"Match";
        dateString = match.captured(1);
        timeString = match.captured(2);
    }
    QString SubStr = "日期:" + dateString + " 时间:" + timeString;
    return SubStr;
}
QString General::GetFileFolder(const QString& OriginStr)
{
    QFileInfo fileinfo(OriginStr);
    return fileinfo.absolutePath();
}
QString General::CreateSaveFileFolder(const QString& OriginStr,const QString& FolderName)
{
    QString FileOriginFolder = General::GetFileFolder(OriginStr);
    QString SaveFileFolderPath = FileOriginFolder + '/' + FolderName;
    return SaveFileFolderPath;
}

bool General::areSameDayandTime(const QString& fileName1,const QString& fileName2)
{
    QRegularExpression regex(R"(_(\d{8})_(\d{6})_)");
    QRegularExpressionMatch match1 = regex.match(fileName1);
    if(!match1.hasMatch())
    {
        qDebug()<<"failed to extract day and time";
        return false;
    }
    QString dateString1 = match1.captured(1);
    QString timeString1 = match1.captured(2);

    QRegularExpressionMatch match2 = regex.match(fileName2);
    if(!match2.hasMatch())
    {
        qDebug()<<"failed to extract day and time";
        return false;
    }
    QString dateString2 = match2.captured(1);
    QString timeString2 = match2.captured(2);


    QString dateTimeFormat = "yyyyMMdd_HHmmss";
    QDateTime dateTime1 = QDateTime::fromString(dateString1+"_"+timeString1,dateTimeFormat);
    QDateTime dateTime2 = QDateTime::fromString(dateString2+"_"+timeString2,dateTimeFormat);
    return dateTime1 == dateTime2;

}
bool General::isTimeDifferenceWithIn30Second(const QString& fileName1,const QString& fileName2)
{
    QRegularExpression regex(R"(_(\d{8})_(\d{6})_)");
    QRegularExpressionMatch match1 = regex.match(fileName1);
    if(!match1.hasMatch())
    {
        qDebug()<<"failed to extract day and time";
        return false;
    }
    QString dateString1 = match1.captured(1);
    QString timeString1 = match1.captured(2);

    QRegularExpressionMatch match2 = regex.match(fileName2);
    if(!match2.hasMatch())
    {
        qDebug()<<"failed to extract day and time";
        return false;
    }
    QString dateString2 = match2.captured(1);
    QString timeString2 = match2.captured(2);


    QString dateTimeFormat = "yyyyMMdd_HHmmss";
    QDateTime dateTime1 = QDateTime::fromString(dateString1+"_"+timeString1,dateTimeFormat);
    QDateTime dateTime2 = QDateTime::fromString(dateString2+"_"+timeString2,dateTimeFormat);

    qint64 secondsDifference = dateTime1.secsTo(dateTime2);
    return myabs(secondsDifference) <= 30;
}


/****************************General Calc********************************/
double ploy(double x,const QVector<double>& Coeff)
{
    return Coeff[0] + Coeff[1]*x + Coeff[2]*x*x +Coeff[3]*x*x*x;
}
double myabs(double x)
{
    if(x >= 0)
    {
        return x;
    }
    else
    {
        return -x;
    }
}

double myarctan(double x)
{
    return qAtan(x);
}
double mycos(double x)
{
    return qCos(x);
}
double mysin(double x)
{
    return qSin(x);
}
//Data output process
void General::DBGRecbin2Mat(const QString& DBGFile1,const QString& DBGFile2,const QString& SaveFolderPath,const QString& Platform)
{
    QProcess process;
    QString CSV2MAT_EXE = "./CSV2MAT.exe";
    QStringList arguments;
    qDebug()<<"CSV2MAT_ProcessStart";
    arguments<<"--DBGCSVFile1"<<DBGFile1<<"--DBGCSVFile2"<<DBGFile2<<"--IssueID"<<SaveFolderPath<<"--Platform"<<Platform;
    process.start(CSV2MAT_EXE,arguments);
    if(!process.waitForStarted())
    {
        qDebug()<<"CSV2MAT_Failed to Start process";
        return;
    }
    process.waitForFinished();
}
void General::CanRecbin2Mat(const QString& CanFile1,const QString& CanFile2,const QString& DBCFile,const QString& LogSignal,const QString& SaveFolderPath,const QString& Platform)
{
    QProcess process;
    QString CAN2MAT_EXE = "./CanRecBin2MAT.exe ";
    qDebug()<<"CanRecBin2MAT_ProcessStart";
    QStringList arguments;
    arguments<<"--DBCFile1"<<DBCFile<<"--CanRecbinfile1"<<CanFile1<<"--CanRecbinfile2"<<CanFile2<<"--NeedLogSignal"<<LogSignal<<"--Platform"<<Platform<<"--IssueID"<<SaveFolderPath;
    qDebug()<<"--DBCFile1"<<DBCFile<<"--CanRecbinfile1"<<CanFile1<<"--CanRecbinfile2"<<CanFile2<<"--NeedLogSignal"<<LogSignal<<"--Platform"<<Platform<<"--IssueID"<<SaveFolderPath;
    process.start(CAN2MAT_EXE,arguments);
    if(!process.waitForStarted())
    {
        qDebug()<<"CanRecBin2MAT_Failed to Start process";
        return;
    }
    process.waitForFinished();
}

void General::CreateIssueDescriptionFile(const QString& folderPath,const QString& fileName,const QString& Content)
{
    QDir dir(folderPath);
    if (!dir.exists())
    {
        qDebug()<<"folder not exist";
        if(!dir.mkpath(folderPath))
        {
            qDebug()<<"folder Create fail";
            return;
        }
    }
    QString fullfilepath  = dir.filePath(fileName);
    QFile file(fullfilepath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"file not open and write";
        return;
    }
    QTextStream out(&file);
    out << Content;
    file.close();
}



QString General::EnumLanePosition2LaneType(E_LanePosition Pos)
{
    QString str1;
    if(Pos == E_LanePosition::SecClsLe)
    {
        str1 = "SecClsLe.";
    }
    else if (Pos == E_LanePosition::ClsLe)
    {
        str1 = "ClsLe.";
    }
    else if (Pos == E_LanePosition::ClsRi)
    {
        str1 = "ClsRi.";
    }
    else
    {
        str1 = "SecClsRi.";
    }
    return str1;
}
QString General::EnumPlatform2SysPlatform(E_Platform Platform)
{
    if(E_Platform::FVC2 == Platform)
    {
        return "FVC2";
    }
    else if (E_Platform::FVC2_1 == Platform)
    {
        return "FVC2.1";
    }
    else if (E_Platform::FVC2_8 == Platform) {
        return "FVC2.8";
    }
    else if (E_Platform::FVC3 == Platform) {
        return "FVC3";
    }
    else if (E_Platform::FVC3_1 == Platform) {
        return "FVC3.1";
    }
    else
    {
        return "FVC3";
    }
}
QString General::EnumLanePosition2EdgeType(E_LanePosition Pos)
{
    if(E_LanePosition::ClsLe == Pos)
    {
        return "FrontLeft";
    }
    else if(E_LanePosition::ClsRi == Pos)
    {
        return "FrontRight";
    }
    else
    {
        return "FrontRight";
    }
}


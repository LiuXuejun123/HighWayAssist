#include "pahhist.h"
#include "CustomDataType.h"
PahHist::PahHist()
{
    this->Xdata.resize(30);
    this->Xdata.fill(0);
    this->Ydata.resize(30);
    this->Ydata.fill(0);
    this->Target_ID = 0;
    this->TrailState = 0;
}
void PahHist::Update(const QStringList& Csvdata,const QStringList& Header,int ID,E_Platform Platform)
{
    if (Platform == E_Platform::FVC3){
        QString str1 = "PahHistEle[" + QString::number(ID)+"]";
        for(int i =0;i<this->Xdata.size();i++)
        {
            QString str2 = str1 +".PosnLgt[" +QString::number(i) + "]";
            QString str3 = str1 +".PosnLat[" +QString::number(i) + "]";
            this->Xdata[i] = Csvdata[Header.indexOf(str2)].toDouble();
            this->Ydata[i] = Csvdata[Header.indexOf(str3)].toDouble();
        }
        this->Target_ID = Csvdata[Header.indexOf(str1+".TarIdn")].toInt();
        this->TrailState = Csvdata[Header.indexOf(str1+".Sts")].toInt();//.Sts
    }
    else if (Platform == E_Platform::FVC2_8)
    {
        QString str1 = "pahHistEle[" + QString::number(ID)+"]";
        for(int i =0;i<this->Xdata.size();i++)
        {
            QString str2 = str1 +".PosnLgt[" +QString::number(i) + "]";
            QString str3 = str1 +".PosnLat[" +QString::number(i) + "]";
            this->Xdata[i] = Csvdata[Header.indexOf(str2)].toDouble();
            this->Ydata[i] = Csvdata[Header.indexOf(str3)].toDouble();
        }
        this->Target_ID = Csvdata[Header.indexOf(str1+".TarIdn")].toInt();
        this->TrailState = Csvdata[Header.indexOf(str1+".Sts")].toInt();//.Sts
    }
}
QVector<double> PahHist::GetXdata()
{
    QVector<double> result;
    for(int i=0;i<this->Xdata.size();i++)
    {
        if (this->Xdata[i]!= 0 && this->Ydata[i]!=0)
        {
            result.push_back(this->Xdata[i]);
        }
    }

    return result;
}
QVector<double> PahHist::GetYdata()
{
    QVector<double> result;
    for(int i=0;i<this->Xdata.size();i++)
    {
        if (this->Xdata[i]!= 0 && this->Ydata[i]!=0)
        {
            result.push_back(this->Ydata[i]);
        }
    }

    return result;
}
int PahHist::GetTrailState()
{
    return this->TrailState;
}
QVector<QVector<double>> PahHist::GetTargetReferenceLine()
{
    QVector<QVector<double>> Result;
    for(int i = 0;i<this->Xdata.size();i++)
    {
        double x = this->Xdata[i];
        double y = this->Ydata[i];
        Result.push_back({x,y});
    }
    return Result;
}

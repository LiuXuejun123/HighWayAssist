#include "lanemkr.h"
#include "general.h"
LaneMkr::LaneMkr()
{
    this->ConstCoeff = 0;;
    this->FirstOrderCoeff = 0;
    this->SecondOrderCoeff = 0;
    this->ThirdOrderCoeff = 0;
    this->DstLgtToEnd = 0;
    this->PosLgt = 0;
    this->PosLat = 0;
    this->LaneType = 0;
}
double LaneMkr::ploy(const double x, const QVector<double> &Coeff)
{
    double y = Coeff[0] + Coeff[1] * x + Coeff[2] * x * x + Coeff[3] * x * x * x;
    return y;
}
void LaneMkr::Update(const QStringList& Csvdata,const QStringList& Header,E_LanePosition Pos,E_Platform Platform)
{
    QString laneprefix = General::EnumLanePosition2LaneType(Pos);
    if (Platform == E_Platform::FVC3)
    {
        this->ConstCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.ConstCoeff")].toDouble();
        this->FirstOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.FirstOrderCoeff")].toDouble();
        this->SecondOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.SecondOrderCoeff")].toDouble();
        this->ThirdOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.ThirdOrderCoeff")].toDouble();
        this->DstLgtToEnd = Csvdata[Header.indexOf(laneprefix+"Estimn.DstLgtToEnd")].toDouble();
        this->PosLgt = Csvdata[Header.indexOf(laneprefix+"Estimn.PosLgt")].toDouble();
        this->PosLat = Csvdata[Header.indexOf(laneprefix+"Estimn.PosLat")].toDouble();
        this->LaneType = Csvdata[Header.indexOf(laneprefix+"Info.LaneMkrType")].toInt();
    }
    else if (Platform == E_Platform::FVC2_8)
    {
        this->ConstCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.constCoeff")].toDouble();
        this->FirstOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.firstOrderCoeff")].toDouble();
        this->SecondOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.secondOrderCoeff")].toDouble();
        this->ThirdOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.thirdOrderCoeff")].toDouble();
        this->DstLgtToEnd = Csvdata[Header.indexOf(laneprefix+"Estimn.DstLgtToEnd")].toDouble();
        this->PosLgt = Csvdata[Header.indexOf(laneprefix+"Estimn.StartPointPosX")].toDouble();
        this->PosLat = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.constCoeff")].toDouble();
        this->LaneType = Csvdata[Header.indexOf(laneprefix+"Info.laneMkrType")].toInt();
    }

    this->Coeff = {this->ConstCoeff,this->FirstOrderCoeff,this->SecondOrderCoeff,this->ThirdOrderCoeff};

}

double LaneMkr::CalcLanePloy(double x)
{
    return this->ploy(x,this->Coeff);
}

QVector<double> LaneMkr::GetLaneDataX()
{
    double intervalValue = this->DstLgtToEnd/60;
    QVector<double> data;
    double SlampleValue = this->PosLgt;
    data.push_back(SlampleValue);
    for(int i = 0;i<60;i++)
    {
        SlampleValue += intervalValue;
        data.push_back(SlampleValue);
    }
    return data;
}

QVector<double> LaneMkr::GetLaneDataY()
{
    QVector<double> Ydata;
    QVector<double> Xdata = this->GetLaneDataX();
    for(int i =0;i<Xdata.size();i++)
    {
        double tempvalue = this->ploy(Xdata[i],this->Coeff);
        Ydata.push_back(tempvalue);
    }
    return Ydata;
}

#include "roadedge.h"
#include "general.h"
#include <QStringList>
RoadEdge::RoadEdge()
{
    this->ConstCoeff = 0;
    this->FirstOrderCoeff = 0;
    this->SecondOrderCoeff = 0;
    this->ThirdOrderCoeff = 0;
    this->DstLgtToEnd = 0;
}
void RoadEdge::Update(const QStringList& Csvdata,const QStringList& Header,E_LanePosition Pos,E_Platform Platform)
{
    QString laneprefix  = General::EnumLanePosition2EdgeType(Pos);
    if (Platform == E_Platform::FVC3)
    {
        this->ConstCoeff = Csvdata[Header.indexOf(laneprefix+".Estimn.ConstCoeff")].toDouble();
        this->FirstOrderCoeff = Csvdata[Header.indexOf(laneprefix+".Estimn.FirstOrderCoeff")].toDouble();
        this->SecondOrderCoeff = Csvdata[Header.indexOf(laneprefix+".Estimn.SecondOrderCoeff")].toDouble();
        this->ThirdOrderCoeff = Csvdata[Header.indexOf(laneprefix+".Estimn.ThirdOrderCoeff")].toDouble();
        this->DstLgtToEnd = Csvdata[Header.indexOf(laneprefix+".Estimn.DstLgtToEnd")].toDouble();

    }
    else if (Platform == E_Platform::FVC2_8)
    {
        this->ConstCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.constCoeff")].toDouble();
        this->FirstOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.firstOrderCoeff")].toDouble();
        this->SecondOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.secondOrderCoeff")].toDouble();
        this->ThirdOrderCoeff = Csvdata[Header.indexOf(laneprefix+"Estimn.PolyCoeff.thirdOrderCoeff")].toDouble();
        this->DstLgtToEnd = Csvdata[Header.indexOf(laneprefix+"Estimn.DstLgtToEnd")].toDouble();
    }

    this->Coeff = {this->ConstCoeff,this->FirstOrderCoeff,this->SecondOrderCoeff,this->ThirdOrderCoeff};

}
QVector<double> RoadEdge::GetLaneDataX()
{
    double intervalValue = this->DstLgtToEnd/60;
    QVector<double> data;
    double SlampleValue = 0;
    data.push_back(SlampleValue);
    for(int i = 0;i<60;i++)
    {
        SlampleValue += intervalValue;
        data.push_back(SlampleValue);
    }
    return data;
}
QVector<double> RoadEdge::GetLaneDataY()
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
double RoadEdge::ploy(const double x,const QVector<double>& Coeff)
{
    double y = Coeff[0] + Coeff[1] * x + Coeff[2] * x * x + Coeff[3] * x * x * x;
    return y;
}
RoadBoundary::RoadBoundary()
{
    RoadEdge Left;
    RoadEdge Right;
}



void RoadBoundary::Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform)
{
    this->Left.Update(Csvdata,Header,E_LanePosition::ClsLe,Platform);
    this->Right.Update(Csvdata,Header,E_LanePosition::ClsRi,Platform);
}
QVector<double> RoadBoundary::GetRoadEdgeDataX(E_LanePosition LanePosition)
{
    if(E_LanePosition::ClsLe == LanePosition)
    {
        return this->Left.GetLaneDataX();
    }
    else
    {
        return this->Right.GetLaneDataX();
    }
}
QVector<double> RoadBoundary::GetRoadEdgeDataY(E_LanePosition LanePosition)
{
    if(E_LanePosition::ClsLe == LanePosition)
    {
        return this->Left.GetLaneDataY();
    }
    else
    {
        return this->Right.GetLaneDataY();
    }
}



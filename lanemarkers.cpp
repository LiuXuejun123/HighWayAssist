#include "lanemarkers.h"

LaneMarkers::LaneMarkers()
{

}
void LaneMarkers::Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform)
{
    this->LeftLeft.Update(Csvdata,Header,E_LanePosition::SecClsLe,Platform);
    this->Left.Update(Csvdata,Header,E_LanePosition::ClsLe,Platform);
    this->Right.Update(Csvdata,Header,E_LanePosition::ClsRi,Platform);
    this->RightRight.Update(Csvdata,Header,E_LanePosition::SecClsRi,Platform);
}
QVector<double> LaneMarkers::GetLaneDataX(E_LanePosition LanePosition)
{
    if(LanePosition == E_LanePosition::SecClsLe)
    {
        return this->LeftLeft.GetLaneDataX();
    }
    else if(LanePosition == E_LanePosition::ClsLe)
    {
        return this->Left.GetLaneDataX();
    }
    else if(LanePosition == E_LanePosition::ClsRi)
    {
        return this->Right.GetLaneDataX();
    }
    else if(LanePosition == E_LanePosition::SecClsRi)
    {
        return this->RightRight.GetLaneDataX();
    }
    else
    {
        return this->Left.GetLaneDataX();
    }
}
QVector<double> LaneMarkers::GetLaneDataY(E_LanePosition LanePosition)
{
    if(LanePosition == E_LanePosition::SecClsLe)
    {
        return this->LeftLeft.GetLaneDataY();
    }
    else if(LanePosition == E_LanePosition::ClsLe)
    {
        return this->Left.GetLaneDataY();
    }
    else if(LanePosition == E_LanePosition::ClsRi)
    {
        return this->Right.GetLaneDataY();
    }
    else if(LanePosition == E_LanePosition::SecClsRi)
    {
        return this->RightRight.GetLaneDataY();
    }
    else
    {
        return this->Left.GetLaneDataY();
    }
}
QPointF LaneMarkers::GetHostLaneC0()
{
    return QPointF(this->Left.ConstCoeff,this->Right.ConstCoeff);
}
QPointF LaneMarkers::GetHostLaneC1()
{
    return QPointF(this->Left.FirstOrderCoeff,this->Right.FirstOrderCoeff);
}
QPointF LaneMarkers::GetHostLaneC2()
{
    return QPointF(this->Left.SecondOrderCoeff,this->Right.SecondOrderCoeff);
}



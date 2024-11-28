#include "vehself.h"
#include "general.h"
VehSelf::VehSelf()
{
    this->VLgt = 0;
    this->YawRate = 0;
    this->ALgt = 0;
    this->PosnLgtDelta= 0;
    this->PosnLatDelta= 0;
}
void VehSelf::Update(const QStringList &Csvdata, const QStringList &Header)
{
    this->VLgt = Csvdata[Header.indexOf("VLgt")].toDouble();
    this->YawRate = Csvdata[Header.indexOf("YawRate")].toDouble();
    this->ALgt =  Csvdata[Header.indexOf("ALgt")].toDouble();
    this->PosnLgtDelta=  Csvdata[Header.indexOf("PosnLgtDelta")].toDouble();
    this->PosnLatDelta=  Csvdata[Header.indexOf("PosnLatDelta")].toDouble();
}

double VehSelf::GetFrontMidDistance2Lane(LaneMkr Lane, double VehWhlbase)
{
    double X1 = VehWhlbase;
    double X2 = VehWhlbase*VehWhlbase;
    double X3 = VehWhlbase*VehWhlbase*VehWhlbase;
    double HeadingSlope = Lane.FirstOrderCoeff + Lane.SecondOrderCoeff*X1 + Lane.ThirdOrderCoeff*X2;
    double Heading = myarctan(HeadingSlope);
    double Offset = Lane.ConstCoeff + Lane.FirstOrderCoeff*X1 + Lane.SecondOrderCoeff*X2 + Lane.ThirdOrderCoeff*X3;
    double Result = mycos(Heading)*Offset;
    return Result;
}
QPointF VehSelf::GetFrontTireDistance2Lane(LaneMarkers LaneMarker, double VehWhlbase,double VehWidth)
{
    double LeftLaneDistance = this->GetFrontMidDistance2Lane(LaneMarker.Left,VehWhlbase);
    double RightLaneDistance = this->GetFrontMidDistance2Lane(LaneMarker.Right,VehWhlbase);
    double LeftTire2LeftLane =  LeftLaneDistance - 0.5*VehWidth;
    double RightTire2RightLane = RightLaneDistance +0.5*VehWidth;
    return QPointF(LeftTire2LeftLane,RightTire2RightLane);
}

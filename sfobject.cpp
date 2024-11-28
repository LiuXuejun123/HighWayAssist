#include "sfobject.h"

SFObject::SFObject()
{

}
void SFObject::Update(const QStringList& Csvdata,const QStringList& Header,int ID,double Vehself_Spd,E_Platform Platform)
{
    if(E_Platform::FVC3 == Platform)
    {
        QString str1 = "Obj[" + QString::number(ID) + "].";
        this->PosnLgt = Csvdata[Header.indexOf(str1+"Estimn.PosnLgt")].toDouble();
        this->PosnLat = Csvdata[Header.indexOf(str1+"Estimn.PosnLat")].toDouble();
        this->VLgt = Csvdata[Header.indexOf(str1+"Estimn.VLgt")].toDouble();
        this->VLat = Csvdata[Header.indexOf(str1+"Estimn.VLat")].toDouble();
        this->object_ID = Csvdata[Header.indexOf(str1+"Ppty.Idn")].toInt();
        this->FusionSrc = Csvdata[Header.indexOf(str1+"Ppty.FusnSrc")].toInt();
        this->Length = Csvdata[Header.indexOf(str1+"Info.Length")].toDouble();
        this->Width = Csvdata[Header.indexOf(str1+"Info.Width")].toDouble();
        this->Type_Number = Csvdata[Header.indexOf(str1+"Info.Typ")].toInt();
        this->SideNear = Csvdata[Header.indexOf(str1+"Info.SideNear")].toInt();
    }
    else if (E_Platform::FVC2_8 == Platform)
    {
        QString str1 = "obj[" + QString::number(ID) + "].";
        this->PosnLgt = Csvdata[Header.indexOf(str1+"Estimn.PosnLgt")].toDouble();
        this->PosnLat = Csvdata[Header.indexOf(str1+"Estimn.PosnLat")].toDouble();
        this->VLgt = Csvdata[Header.indexOf(str1+"Estimn.VLgt")].toDouble();
        this->VLat = Csvdata[Header.indexOf(str1+"Estimn.VLat")].toDouble();
        this->object_ID = Csvdata[Header.indexOf(str1+"Ppty.Idn")].toInt();
        this->FusionSrc = Csvdata[Header.indexOf(str1+"Ppty.FusnSrc")].toInt();
        this->Length = Csvdata[Header.indexOf(str1+"Info.Length")].toDouble();
        this->Width = Csvdata[Header.indexOf(str1+"Info.Width")].toDouble();
        this->Type_Number = Csvdata[Header.indexOf(str1+"Info.Typ")].toInt();
        this->SideNear = Csvdata[Header.indexOf(str1+"Info.SideNear")].toInt();
    }

    this->calculateTTC(Vehself_Spd);
    this->calculateTimeGap(Vehself_Spd);
}
bool SFObject::isObjectValid()//目标过小 or ID == 0
{
    //if((this->object_ID != 0)&&(this->Width >=0.45)&&(this->Length >= 0.45))
    if(this->object_ID != 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
QPointF SFObject::GetTopLeftPoint()
{
    QPointF RearAxleMid = this->GetObjectPos();
    QPointF point;
    point.setX(RearAxleMid.x()+0.5*this->Width);
    point.setY(RearAxleMid.y()+this->Length);
    return point;
}
QPointF SFObject::GetbottomRightPoint()
{
    QPointF RearAxleMid = this->GetObjectPos();
    QPointF point;
    point.setX(RearAxleMid.x()-0.5*this->Width);
    point.setY(RearAxleMid.y());
    return point;
}
QPointF SFObject::GetObjectPos()
{
    QPointF point;
    if(this->SideNear == 0)
    {
        point.setX(this->PosnLat);
        point.setY(this->PosnLgt);
    }
    else if (this->SideNear == 1)
    {
        point.setX(this->PosnLat);
        point.setY(this->PosnLgt);
    }
    else if (this->SideNear == 2)
    {
        point.setX(this->PosnLat - 0.5*this->Width);
        point.setY(this->PosnLgt - 0.5*this->Length);
    }
    else if (this->SideNear == 3)
    {
        point.setX(this->PosnLat + 0.5*this->Width);
        point.setY(this->PosnLgt - 0.5*this->Length);
    }
    else
    {
        point.setX(this->PosnLat);
        point.setY(this->PosnLgt);
    }
    return point;
}





E_FusionSrc SFObject::PlotColorMode()
{
    bool frontvision = this->FusionSrc & 1;
    bool frontradar = this->FusionSrc & 2;
    bool FL_CornerRadar = this->FusionSrc & 4;
    bool FR_CornerRadar = this->FusionSrc & 8;
    bool RR_CornerRadar = this->FusionSrc & 16;
    bool RL_CornerRadar = this->FusionSrc & 32;
    if (frontvision)
    {
        if (frontradar)//前雷达视觉融合
        {
            return E_FusionSrc::FrontVision_FrontRadar;
        }
        else if (FL_CornerRadar || FR_CornerRadar || RR_CornerRadar || RL_CornerRadar )//前视觉与角雷达融合
        {
            return E_FusionSrc::FrontVision_CornerRadar;
        }
        else // 单视觉
        {
            return E_FusionSrc::FrontVision;
        }
    }
    else
    {
        if (frontradar)
        {
            return E_FusionSrc::FrontRadar;
        }
        else if (FL_CornerRadar || FR_CornerRadar || RR_CornerRadar || RL_CornerRadar )
        {
            return E_FusionSrc::CornerRadar;
        }
        else
        {
            return E_FusionSrc::None;
        }
    }

}

QString SFObject::GetObjectType()
{
    if(this->Type_Number == 0)
    {
        return "UkwnClass";
    }
    else if (this->Type_Number == 1)
    {
        return "Car";
    }
    else if (this->Type_Number == 2)
    {
        return "Motorcycle";
    }
    else if (this->Type_Number == 3)
    {
        return "Truck";
    }
    else if (this->Type_Number == 4)
    {
        return "Ped";
    }
    else if (this->Type_Number == 7)
    {
        return "Anim";
    }
    else if (this->Type_Number == 8)
    {
        return "ObjGen";
    }
    else if (this->Type_Number == 9)
    {
        return "Bicycle";
    }
    else if (this->Type_Number == 10)
    {
        return "VehOfUkwnClass";
    }
    else if (this->Type_Number == 11)
    {
        return "Bus";
    }
    else
    {
        return "UkwnClass";
    }
}

void SFObject::CalcDistance2Lane(LaneMarkers LaneMarker)
{
    double Dist2Left = 20;
    double Dist2Right = -20;
    double Dist2LeftLeft = 40;
    double Dist2RightRight = -40;
    if(LaneMarker.LeftLeft.DstLgtToEnd>=5)
    {
        Dist2LeftLeft = LaneMarker.LeftLeft.CalcLanePloy(this->PosnLgt);
    }
    if(LaneMarker.Left.DstLgtToEnd>=5)
    {
        Dist2Left = LaneMarker.Left.CalcLanePloy(this->PosnLgt);
    }
    if(LaneMarker.Right.DstLgtToEnd>= 5)
    {
        Dist2Right = LaneMarker.Right.CalcLanePloy(this->PosnLgt);
    }
    if(LaneMarker.RightRight.DstLgtToEnd >= 5)
    {
        Dist2RightRight = LaneMarker.RightRight.CalcLanePloy(this->PosnLgt);
    }
    QVector<double> Data;
    Data.push_back(Dist2LeftLeft);
    Data.push_back(Dist2Left);
    Data.push_back(Dist2Right);
    Data.push_back(Dist2RightRight);


    QPair<double, double> pair;
    for(int i =0;i<Data.size()-1;i++)
    {
        if(this->PosnLat<= Data[i]&&this->PosnLat >= Data[i+1])
        {
            pair = {Data[i], Data[i+1]};
            break;
        }
    }
    if(pair.first==-40||pair.first==-20||pair.first==20||pair.first==40)//default
    {
        this->Dis2LeftLane = 100;
    }
    else
    {
        this->Dis2LeftLane =pair.first - this->PosnLat - 0.5*this->Width;
    }
    if (pair.second==-40||pair.second==-20||pair.second==20||pair.second==40)//default
    {
        this->Dis2RigthLane = 100;
    }
    else
    {
        this->Dis2RigthLane = this->PosnLat - pair.second - 0.5*this->Width;
    }
    //    qDebug()<< "Dis2RightLane"
}


void SFObject::calculateTTC(double vehself_Spd)
{
    double diffSpd = vehself_Spd - this->VLgt;
    this->TTC = this->PosnLgt/diffSpd;
    double MaxValue =100;
    double MinValue =-100;
    if (this->TTC < MinValue)
    {
        this->TTC = MinValue;
    }
    else if(this->TTC > MaxValue)
    {
        this->TTC = MaxValue;
    }
    else
    {

    }
}
void  SFObject::calculateTimeGap(double vehself_Spd)
{
    this->TG = this->PosnLgt/vehself_Spd;
}
QVector<QString> SFObject::GetObjectInfo(LaneMarkers LaneMarker)
{
    this->CalcDistance2Lane(LaneMarker);
    QVector<QString> info;
    info.push_back(QString::number(this->object_ID));
    info.push_back(QString::number(this->PosnLgt));
    info.push_back(QString::number(this->PosnLat));
    info.push_back(QString::number(this->VLgt));
    info.push_back(QString::number(this->VLat));
    info.push_back(this->GetObjectType());
    info.push_back(QString::number(this->TTC));
    info.push_back(QString::number(this->Dis2LeftLane));
    info.push_back(QString::number(this->Dis2RigthLane));
    info.push_back(QString::number(this->Dis2Host));
    return info;
}

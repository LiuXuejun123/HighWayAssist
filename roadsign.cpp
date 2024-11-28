#include "roadsign.h"
#include <QStringList>
#include <qdebug.h>
QPointF StopLine::GetTopLeftPoint()
{
    QPointF Result(this->Left.y(),this->Left.x()+0.5);
    return Result;
}

QPointF StopLine::GetbottomRightPoint()
{
    QPointF Result(this->Right.y(),this->Right.x());
    return Result;
}
bool StopLine::IsValid()
{
    if(this->ID==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QPointF CrossLine::GetTopLeftPoint()
{
    QPointF Result(this->LU.y(),this->LU.x());
    return Result;
}


QPointF CrossLine::GetbottomRightPoint()
{
    QPointF Result(this->RD.y(),this->RD.x());
    return Result;
}
bool CrossLine::IsValid()
{
    if(this->ID==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}



RoadSign::RoadSign()
{
    QVector<StopLine> SL{8};
    QVector<CrossLine> CL{4};
    QVector<RoadArrow> RA{8};
}
void RoadSign::Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform)
{
    if(E_Platform::FVC3 == Platform)
    {
        //StopLane
        qDebug()<<"StopLane";
        for(int i =0 ;i<this->SL.size();i++)
        {
            QString StopLanePrePix = "SLList["+ QString::number(i) +"].";
            this->SL[i].ID = Csvdata[Header.indexOf(StopLanePrePix+"ID")].toInt();
            this->SL[i].Left.setX(Csvdata[Header.indexOf(StopLanePrePix+"LeftPointPosLgt")].toDouble());
            this->SL[i].Left.setY(Csvdata[Header.indexOf(StopLanePrePix+"LeftPointPosLat")].toDouble());
            this->SL[i].Right.setX(Csvdata[Header.indexOf(StopLanePrePix+"RightPointPosLgt")].toDouble());
            this->SL[i].Right.setY(Csvdata[Header.indexOf(StopLanePrePix+"RightPointPosLat")].toDouble());
            this->SL[i].Confidence = Csvdata[Header.indexOf(StopLanePrePix+"Confi")].toInt();
        }
        //CrossRoad
        qDebug()<<"CrossLane";
        for(int j = 0;j<this->CL.size();j++)
        {
            QString CrossLanePrePix  = "CWList["+ QString::number(j) +"].";
            this->CL[j].ID = Csvdata[Header.indexOf(CrossLanePrePix+"ID")].toInt();
            this->CL[j].LU.setX(Csvdata[Header.indexOf(CrossLanePrePix+"LuPointPosLgt")].toDouble());
            this->CL[j].LU.setY(Csvdata[Header.indexOf(CrossLanePrePix+"LuPointPosLat")].toDouble());
            this->CL[j].RU.setX(Csvdata[Header.indexOf(CrossLanePrePix+"RuPointPosLgt")].toDouble());
            this->CL[j].RU.setY(Csvdata[Header.indexOf(CrossLanePrePix+"RuPointPosLat")].toDouble());
            this->CL[j].RD.setX(Csvdata[Header.indexOf(CrossLanePrePix+"RdPointPosLgt")].toDouble());
            this->CL[j].RD.setY(Csvdata[Header.indexOf(CrossLanePrePix+"RdPointPosLat")].toDouble());
            this->CL[j].LD.setX(Csvdata[Header.indexOf(CrossLanePrePix+"LdPointPosLgt")].toDouble());
            this->CL[j].LD.setY(Csvdata[Header.indexOf(CrossLanePrePix+"LdPointPosLat")].toDouble());
            this->CL[j].Confidence =  Csvdata[Header.indexOf(CrossLanePrePix+"Confi")].toInt();
        }
        //
        qDebug()<<"RoadArrow";
        for(int k = 0;k<this->RA.size();k++)
        {
            QString RoadArrow = "RAList["+ QString::number(k) +"].";
            this->RA[k].ID = Csvdata[Header.indexOf(RoadArrow+"ID")].toInt();
            this->RA[k].Type = Csvdata[Header.indexOf(RoadArrow+"Type")].toInt();
            this->RA[k].Confidence = Csvdata[Header.indexOf(RoadArrow+"Confi")].toInt();
            this->RA[k].LaneAssignment = Csvdata[Header.indexOf(RoadArrow+"LaneAssignment")].toInt();
            this->RA[k].PosLgt = Csvdata[Header.indexOf(RoadArrow+"PosLgt")].toDouble();
            this->RA[k].PosLat = Csvdata[Header.indexOf(RoadArrow+"PosLat")].toDouble();
        }
    }
    else if (E_Platform::FVC3_1 == Platform)
    {
        //StopLane
        for(int i =0 ;i<this->SL.size();i++)
        {
            QString StopLanePrePix = "SLList["+ QString::number(i) +"].";
            this->SL[i].ID = Csvdata[Header.indexOf(StopLanePrePix+"ID")].toInt();
            this->SL[i].Left.setX(Csvdata[Header.indexOf(StopLanePrePix+"LeftPointPosLgt")].toDouble());
            this->SL[i].Left.setY(Csvdata[Header.indexOf(StopLanePrePix+"LeftPointPosLat")].toDouble());
            this->SL[i].Right.setX(Csvdata[Header.indexOf(StopLanePrePix+"RightPointPosLgt")].toDouble());
            this->SL[i].Right.setY(Csvdata[Header.indexOf(StopLanePrePix+"RightPointPosLat")].toDouble());
            this->SL[i].Confidence = Csvdata[Header.indexOf(StopLanePrePix+"Confi")].toInt();
        }
        //CrossRoad
        for(int j = 0;j<this->CL.size();j++)
        {
            QString CrossLanePrePix  = "CWList["+ QString::number(j) +"].";
            this->CL[j].ID = Csvdata[Header.indexOf(CrossLanePrePix+"ID")].toInt();
            this->CL[j].LU.setX(Csvdata[Header.indexOf(CrossLanePrePix+"LuPointPosLgt")].toDouble());
            this->CL[j].LU.setY(Csvdata[Header.indexOf(CrossLanePrePix+"LuPointPosLat")].toDouble());
            this->CL[j].RU.setX(Csvdata[Header.indexOf(CrossLanePrePix+"RuPointPosLgt")].toDouble());
            this->CL[j].RU.setY(Csvdata[Header.indexOf(CrossLanePrePix+"RuPointPosLat")].toDouble());
            this->CL[j].RD.setX(Csvdata[Header.indexOf(CrossLanePrePix+"RdPointPosLgt")].toDouble());
            this->CL[j].RD.setY(Csvdata[Header.indexOf(CrossLanePrePix+"RdPointPosLat")].toDouble());
            this->CL[j].LD.setX(Csvdata[Header.indexOf(CrossLanePrePix+"LdPointPosLgt")].toDouble());
            this->CL[j].LD.setY(Csvdata[Header.indexOf(CrossLanePrePix+"LdPointPosLat")].toDouble());
            this->CL[j].Confidence =  Csvdata[Header.indexOf(CrossLanePrePix+"Confi")].toInt();
        }
        //
        for(int k = 0;k<this->RA.size();k++)
        {
            QString RoadArrow = "RAList["+ QString::number(k) +"].";
            this->RA[k].ID = Csvdata[Header.indexOf(RoadArrow+"ID")].toInt();
            this->RA[k].Type = Csvdata[Header.indexOf(RoadArrow+"Type")].toInt();
            this->RA[k].Confidence = Csvdata[Header.indexOf(RoadArrow+"Confi")].toInt();
            this->RA[k].LaneAssignment = Csvdata[Header.indexOf(RoadArrow+"LaneAssignment")].toInt();
            this->RA[k].PosLgt = Csvdata[Header.indexOf(RoadArrow+"PosLgt")].toDouble();
            this->RA[k].PosLat = Csvdata[Header.indexOf(RoadArrow+"PosLat")].toDouble();
        }
    }

}


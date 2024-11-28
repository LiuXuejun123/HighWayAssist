#ifndef ROADSIGN_H
#define ROADSIGN_H
#include <QPointF>
#include <QVector>
#include "CustomDataType.h"
class StopLine
{
public:
    QPointF Left;
    QPointF Right;
    int ID;
    int Confidence;
    bool IsValid();
    QPointF GetTopLeftPoint();
    QPointF GetbottomRightPoint();
};
class CrossLine
{
public:
    int ID;
    int Confidence;
    QPointF LU;
    QPointF RU;
    QPointF LD;
    QPointF RD;
    bool IsValid();
    QPointF GetTopLeftPoint();
    QPointF GetbottomRightPoint();
};
class RoadArrow
{
public:
    int ID;
    int Type;
    int Confidence;
    int LaneAssignment;
    double PosLgt;
    double PosLat;
};
class RoadSign
{
public:
    QVector<StopLine> SL{8};
    QVector<CrossLine> CL{4};
    QVector<RoadArrow> RA{8};
    RoadSign();
    void Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform);
};

#endif // ROADSIGN_H

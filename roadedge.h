#ifndef ROADEDGE_H
#define ROADEDGE_H
#include <QString>
#include "CustomDataType.h"
#include <QVector>
class RoadEdge
{
public:
    double ConstCoeff;
    double FirstOrderCoeff;
    double SecondOrderCoeff;
    double ThirdOrderCoeff;
    double DstLgtToEnd;

    RoadEdge();
    void Update(const QStringList& Csvdata,const QStringList& Header,E_LanePosition Pos,E_Platform Platform);
    QVector<double> GetLaneDataX();
    QVector<double> GetLaneDataY();
private:
    QVector<double> Coeff{4};
    double ploy(const double x,const QVector<double>& Coeff);
};

class RoadBoundary
{
public:
    RoadEdge Left;
    RoadEdge Right;
    RoadBoundary();
    void Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform);
    QVector<double> GetRoadEdgeDataX(E_LanePosition LanePosition);
    QVector<double> GetRoadEdgeDataY(E_LanePosition LanePosition);
};
#endif // ROADEDGE_H


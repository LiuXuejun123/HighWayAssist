#ifndef LANEMARKERS_H
#define LANEMARKERS_H
#include "lanemkr.h"
#include <QPointF>
class LaneMarkers
{
public:
    LaneMkr LeftLeft;
    LaneMkr Left;
    LaneMkr Right;
    LaneMkr RightRight;
    LaneMarkers();
    void Update(const QStringList& Csvdata,const QStringList& Header,E_Platform Platform);
    QVector<double> GetLaneDataX(E_LanePosition LanePosition);
    QVector<double> GetLaneDataY(E_LanePosition LanePosition);
    QPointF GetHostLaneC0();
    QPointF GetHostLaneC1();
    QPointF GetHostLaneC2();
private:

};

#endif // LANEMARKERS_H

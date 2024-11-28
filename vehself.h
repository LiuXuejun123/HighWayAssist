#ifndef VEHSELF_H
#define VEHSELF_H
#include "lanemarkers.h"
#include "lanemkr.h"
#include <QString>
#include <QStringList>
#include <QPoint>

class VehSelf
{
public:
    double VLgt;
    double YawRate;
    double ALgt;
    double PosnLgtDelta;
    double PosnLatDelta;
    VehSelf();
    void Update(const QStringList &Csvdata, const QStringList &Header);
    QPointF GetFrontTireDistance2Lane(LaneMarkers LaneMarker, double VehWhlbase,double VehWidth);

private:
    double GetFrontMidDistance2Lane(LaneMkr Lane, double VehWhlbase);
};

#endif // VEHSELF_H

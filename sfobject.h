#ifndef SFOBJECT_H
#define SFOBJECT_H
#include "lanemarkers.h"
#include "CustomDataType.h"
class SFObject
{
public:
    double PosnLgt;
    double PosnLat;
    double Width;
    double Length;
    double VLgt;
    double VLat;
    int FusionSrc;
    int SideNear;
    int object_ID;
    int Type_Number;

    double Dis2LeftLane;
    double Dis2RigthLane;
    double Dis2Host;
    double TG;
    double TTC;

    SFObject();
    void Update(const QStringList& Csvdata,const QStringList& Header,int ID,double Vehself_Spd,E_Platform Platform);
    bool isObjectValid();//目标是否有效
    E_FusionSrc PlotColorMode();//SensorFusion
    QPointF GetTopLeftPoint();
    QPointF GetbottomRightPoint();
    QPointF GetObjectPos();
    QString GetObjectType();
    void CalcDistance2Lane(LaneMarkers LaneMarker);
    QVector<QString> GetObjectInfo(LaneMarkers LaneMarker);
private:
    void calculateTTC(double vehself_spd);
    void calculateTimeGap(double vehself_spd);
};

#endif // SFOBJECT_H


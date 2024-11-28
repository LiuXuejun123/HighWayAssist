#ifndef LANEMKR_H
#define LANEMKR_H
#include <QVector>
#include "CustomDataType.h"

class LaneMkr
{
public:

    double ConstCoeff;
    double FirstOrderCoeff;
    double SecondOrderCoeff;
    double ThirdOrderCoeff;
    double DstLgtToEnd;
    double PosLgt;
    double PosLat;
    int LaneType;
    QVector<double> Coeff{4};
    LaneMkr();
    void Update(const QStringList& Csvdata,const QStringList& Header,E_LanePosition Pos,E_Platform Platform);
    double CalcLanePloy(double x);
    QVector<double> GetLaneDataX();
    QVector<double> GetLaneDataY();
private:
    double ploy(const double x,const QVector<double>& Coeff);
};

#endif // LANEMKR_H

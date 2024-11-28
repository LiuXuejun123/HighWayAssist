#ifndef PAHHIST_H
#define PAHHIST_H
#include <QDebug>
#include <QVector>
#include "general.h"
class PahHist
{
public:
    PahHist();
    void Update(const QStringList& Csvdata,const QStringList& Header,int ID,E_Platform Platform);
    QVector<double> GetXdata();
    QVector<double> GetYdata();
    int GetTrailState();
    QVector<QVector<double>> GetTargetReferenceLine();
private:
    QVector<double> Xdata{30};
    QVector<double> Ydata{30};
    int Target_ID;
    int TrailState;
};

#endif // PAHHIST_H

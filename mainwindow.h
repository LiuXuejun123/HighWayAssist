#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "general.h"
#include "lanemarkers.h"
#include "mysqlite.h"
#include "pahhist.h"
#include "roadedge.h"
#include "sfobject.h"
#include "vehself.h"
#include "CustomDataType.h"
#include "roadsign.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private://private func
    void ComboxCheck();
    void FileOpenAction();
    void SysInit();
    void DataUpdate(int index);
    void CycleFunction();
    void PlotRoadEdge();
    void PlotStopLane();
    void PlotCrossLane();
    void PlotView();
    void PlotLane();
    void PlotPath();
    void PlotObject();
    void LCDReflash();
    void TableViewUpdate();
    void SilderReset();
    void Silderflash();
    void SilderReleasedAction();
    void ConnectAction2Function();
private slots:
    void on_StartButton_clicked();

    void on_PauseButton_clicked();

    void on_PrevButton_clicked();

    void on_NextButton_clicked();

    void on_DBGFile1Button_clicked();

    void on_DBGFile2Button_clicked();

    void on_CanFile1Button_clicked();

    void on_CanFile2Button_clicked();

    void on_DBCFileButton_clicked();

    void on_LogSignalButton_clicked();

    void ReflashResimTextEdit();

    void ExportSingleFile();

    void ExportDoubleFile();

    void PaCheckBoxState(int state);

    void GridCheckBoxState(int state);

    void LKACheckBoxState(int state);

    void ELKACheckBoxState(int state);

    void LDWCheckBoxState(int state);

    void DCLCCheckBoxState(int state);

    void ALCCheckBoxState(int state);

    void SmartOffsetCheckBoxState(int state);
private:
    Ui::MainWindow *ui;
    QTimer* SysTimer;
    int currentindex = 0;
    int maxindex = 0;
    E_Platform Platform;
    QString DBGfilename1;
    QString DBGfilename2;
    QString DBCfilename;
    QString Canfilename1;
    QString Canfilename2;
    QString CanLogSignalfilename;
    QString MatFileSaveFolder;
    //Data
    QVector<QStringList> LaneMkrData;
    QStringList LaneMkrHeader;
    QVector<QStringList> ObjectData;
    QStringList ObjectHeader;
    QVector<QStringList> PahHistData;
    QStringList PahHistHeader;
    QVector<QStringList> VehSelfData;
    QStringList VehSelfHeader;
    QVector<QStringList> RoadEdgeData;
    QStringList RoadEdgeHeader;
    QVector<QStringList> RoadSignData;
    QStringList RoadSignHeader;
    QVector<SFObject> obj{32};
    VehSelf vehself;
    QVector<PahHist> Path{4};
    LaneMarkers LaneMarker;
    RoadBoundary RoadEdge;
    RoadSign RoadSigns;
    //CheckBox
    bool PAmode  = false;
    bool LKAmode= false;
    bool ELKAmode= false;
    bool LDWmode= false;
    bool DCLCmode= false;
    bool ALCmode= false;
    bool SmartOffsetmode= false;
    bool Grid= false;


};
#endif // MAINWINDOW_H


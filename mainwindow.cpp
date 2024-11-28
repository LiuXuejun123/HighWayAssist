#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentWidget(ui->tab);
    // 定时器
    this->SysTimer  = new QTimer(this);
    this->SysTimer->setInterval(25);
    // 信号
    this->ConnectAction2Function();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ConnectAction2Function()
{
    connect(ui->actionDBGFile_V,&QAction::triggered,this,&MainWindow::FileOpenAction);
    connect(ui->action_S,&QAction::triggered,this,&MainWindow::ExportSingleFile);
    connect(ui->action_D,&QAction::triggered,this,&MainWindow::ExportDoubleFile);
    connect(this->SysTimer,&QTimer::timeout,this,&MainWindow::CycleFunction);
    connect(ui->TimehorizontalSlider,&QSlider::sliderReleased,this,&MainWindow::SilderReleasedAction);
    connect(ui->PA_checkBox,&QCheckBox::stateChanged,this,&MainWindow::PaCheckBoxState);
    connect(ui->LDW_checkBox,&QCheckBox::stateChanged,this,&MainWindow::LDWCheckBoxState);
    connect(ui->LKA_checkBox,&QCheckBox::stateChanged,this,&MainWindow::LKACheckBoxState);
    connect(ui->ELKA_checkBox,&QCheckBox::stateChanged,this,&MainWindow::ELKACheckBoxState);
    connect(ui->Grid_checkBox,&QCheckBox::stateChanged,this,&MainWindow::GridCheckBoxState);
    connect(ui->DCLC_checkBox,&QCheckBox::stateChanged,this,&MainWindow::DCLCCheckBoxState);
    connect(ui->ALC_checkBox,&QCheckBox::stateChanged,this,&MainWindow::ALCCheckBoxState);
    connect(ui->SmartOffset_checkBox,&QCheckBox::stateChanged,this,&MainWindow::SmartOffsetCheckBoxState);
}
// 按键
void MainWindow::on_StartButton_clicked()
{
    this->SysTimer->start();
}

void MainWindow::on_PauseButton_clicked()
{
    this->SysTimer->stop();
}

void MainWindow::on_PrevButton_clicked()
{
    this->SysTimer->stop();
    this->currentindex -= 1;
    if(this->currentindex < 1)
    {
        QMessageBox::warning(nullptr, "警告", "已经到最开始!");
        return;
    }else
    {
        this->DataUpdate(this->currentindex);
        this->PlotView();
        this->LCDReflash();
        this->Silderflash();
        this->TableViewUpdate();
    }
}

void MainWindow::on_NextButton_clicked()
{
    this->SysTimer->stop();
    this->currentindex += 1;
    if(this->currentindex >= this->maxindex)
    {
        QMessageBox::warning(nullptr, "警告", "已经到最后!");
        this->currentindex = this->maxindex-1;
        return;
    }else
    {
        this->DataUpdate(this->currentindex);
        this->PlotView();
        this->LCDReflash();
        this->Silderflash();
        this->TableViewUpdate();
    }
}

// Resim 按键
void MainWindow::on_DBGFile1Button_clicked()
{
    this->DBGfilename1 = General::SearchFileCSV();
    this->ReflashResimTextEdit();
    this->DBGfilename2 ="None";
    ui->DBGFile2textEdit->clear();
}

void MainWindow::on_DBGFile2Button_clicked()
{
    this->DBGfilename2 = General::SearchFileCSV();
    this->ReflashResimTextEdit();
}

void MainWindow::on_CanFile1Button_clicked()
{
    this->Canfilename1 = General::SearchFileCanRecbin();
    this->ReflashResimTextEdit();
    this->Canfilename2 ="None";
    ui->CanFile2textEdit->clear();
}

void MainWindow::on_CanFile2Button_clicked()
{
    this->Canfilename2 = General::SearchFileCanRecbin();
    this->ReflashResimTextEdit();
}

void MainWindow::on_DBCFileButton_clicked()
{
    this->DBCfilename = General::SearchFileDBC();
    this->ReflashResimTextEdit();
}

void MainWindow::on_LogSignalButton_clicked()
{
    this->CanLogSignalfilename = General::SearchFileXlsx();
    this->ReflashResimTextEdit();
}
void MainWindow::ExportSingleFile()
{
    QString IssueID = ui->IssueIDtextEdit->toPlainText();
    if (IssueID == "")
    {
        QMessageBox::information(nullptr, "提示", "请输入问题ID!");
        return;
    }

    QString Description = ui->IssueDescriptiontextEdit->toPlainText();
    if (Description == "")
    {
        QMessageBox::information(nullptr, "提示", "请输入问题描述!");
        return;
    }

    if(this->DBCfilename == "" ||this->DBCfilename == "None"  )
    {
        QMessageBox::information(nullptr, "提示", "请输入DBC!");
        return;
    }

    if(this->DBGfilename1 == "" ||this->DBGfilename1 == "None" )
    {
        QMessageBox::information(nullptr, "提示", "请选择DBG文件");
        return;
    }
    else
    {
        this->MatFileSaveFolder = General::CreateSaveFileFolder(this->DBGfilename1,IssueID);
        if(this->Canfilename1 == "" ||this->Canfilename1 == "None" )
        {
            QMessageBox::StandardButton reply =   QMessageBox::question(nullptr, "提示", "是否需要导出DBG",QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                return;
            }
            else
            {
                //描述文件
                General::CreateIssueDescriptionFile(this->MatFileSaveFolder,"Description.txt",Description);
                QMessageBox::information(nullptr, "提示", "描述文件已写入");
                //导出DBG
                General::DBGRecbin2Mat(this->DBGfilename1,"None",this->MatFileSaveFolder,General::EnumPlatform2SysPlatform(this->Platform));
                QMessageBox::information(nullptr, "提示", "DBG文件已写入");
            }
        }
        else//
        {
            bool isatSameTime = General::areSameDayandTime(this->DBGfilename1,this->Canfilename1);
            if(isatSameTime)
            {
                //描述文件
                General::CreateIssueDescriptionFile(this->MatFileSaveFolder,"Description.txt",Description);
                QMessageBox::information(nullptr, "提示", "描述文件已写入");
                //导出DBG
                General::DBGRecbin2Mat(this->DBGfilename1,"None",this->MatFileSaveFolder,General::EnumPlatform2SysPlatform(this->Platform));
                QMessageBox::information(nullptr, "提示", "DBG文件已写入");
                //导出Can
                General::CanRecbin2Mat(this->Canfilename1,"None",this->DBCfilename,this->CanLogSignalfilename,this->MatFileSaveFolder,General::EnumPlatform2SysPlatform(this->Platform));
                QMessageBox::information(nullptr, "提示", "Can文件已写入");
            }
            else
            {
                QMessageBox::information(nullptr, "提示", "Can文件与DBG文件时间不一致");
                return;
            }
        }
    }
}
void MainWindow::ExportDoubleFile()
{
    QString IssueID = ui->IssueIDtextEdit->toPlainText();
    if (IssueID == "")
    {
        QMessageBox::information(nullptr, "提示", "请输入问题ID!");
        return;
    }

    QString Description = ui->IssueDescriptiontextEdit->toPlainText();
    if (Description == "")
    {
        QMessageBox::information(nullptr, "提示", "请输入问题描述!");
        return;
    }

    if(this->DBCfilename == "" ||this->DBCfilename == "None"  )
    {
        QMessageBox::information(nullptr, "提示", "请输入DBC!");
        return;
    }
    // DBG double
    if(this->DBGfilename1 == "" ||this->DBGfilename1 == "None" || this->DBGfilename2 == "" ||this->DBGfilename2 == "None"  )
    {
        QMessageBox::information(nullptr, "提示", "请选择两个DBG文件");
        return;
    }
    else
    {
        if(this->DBGfilename1 != "" && this->DBGfilename1 != "None" && this->DBGfilename2 != "" && this->DBGfilename2 != "None" )
        {
            if(!(General::isTimeDifferenceWithIn30Second(this->DBGfilename1,this->DBGfilename2)))
            {
                QMessageBox::information(nullptr, "提示", "请确认两个文件的间隔时间");
                return;
            }
        }

    }
    // Can Double
    if(this->Canfilename1 == "" ||this->Canfilename1 == "None" || this->Canfilename2 == "" ||this->Canfilename2 == "None"  )
    {
        QMessageBox::information(nullptr, "提示", "请选择两个Can文件");
        return;
    }
    else
    {
        if(this->Canfilename1 != "" && this->Canfilename1 != "None" && this->Canfilename2 != "" && this->Canfilename2 != "None" )
        {
            if(!(General::isTimeDifferenceWithIn30Second(this->Canfilename1,this->Canfilename2)))
            {
                QMessageBox::information(nullptr, "提示", "请确认两个文件的间隔时间");
                return;
            }
        }

    }
    // Double Same Record Time
    if(General::areSameDayandTime(this->DBGfilename1,this->Canfilename1) && General::areSameDayandTime(this->DBGfilename2,this->Canfilename2))
    {
        this->MatFileSaveFolder = General::CreateSaveFileFolder(this->DBGfilename1,IssueID);
        //描述文件
        General::CreateIssueDescriptionFile(this->MatFileSaveFolder,"Description.txt",Description);
        QMessageBox::information(nullptr, "提示", "描述文件已写入");
        //导出DBG
        General::DBGRecbin2Mat(this->DBGfilename1,this->DBGfilename2,this->MatFileSaveFolder,General::EnumPlatform2SysPlatform(this->Platform));
        QMessageBox::information(nullptr, "提示", "DBG文件已写入");
        //导出Can
        General::CanRecbin2Mat(this->Canfilename1,this->Canfilename2,this->DBCfilename,this->CanLogSignalfilename,this->MatFileSaveFolder,General::EnumPlatform2SysPlatform(this->Platform));
        QMessageBox::information(nullptr, "提示", "Can文件已写入");
    }
    else
    {
        QMessageBox::information(nullptr, "提示", "DBG 与 Can 文件时间戳不一致");
        return;
    }

}


void MainWindow::ReflashResimTextEdit()
{
    ui->DBGFile1textEdit->setText(this->DBGfilename1);
    ui->DBGFile2textEdit->setText(this->DBGfilename2);
    ui->DBCFiletextEdit->setText(this->DBCfilename);
    ui->CanFile1textEdit->setText(this->Canfilename1);
    ui->CanFile2textEdit->setText(this->Canfilename2);
    ui->LogSignaltextEdit->setText(this->CanLogSignalfilename);
}



// checkbox state
void MainWindow::PaCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->PAmode = true;
    }
    else
    {
        this->PAmode = false;
    }
}

void MainWindow::LKACheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->LKAmode = true;
    }
    else
    {
        this->LKAmode = false;
    }
}

void MainWindow::LDWCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->LDWmode = true;
    }
    else
    {
        this->LDWmode = false;
    }
}

void MainWindow::ELKACheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->ELKAmode = true;
    }
    else
    {
        this->ELKAmode = false;
    }
}

void MainWindow::DCLCCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->DCLCmode = true;
    }
    else
    {
        this->DCLCmode = false;
    }
}


void MainWindow::ALCCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->ALCmode = true;
    }
    else
    {
        this->ALCmode = false;
    }
}
void MainWindow::GridCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->Grid = true;
    }
    else
    {
        this->Grid = false;
    }
}

void MainWindow::SmartOffsetCheckBoxState(int state)
{
    if (state == Qt::Checked)
    {
        this->SmartOffsetmode = true;
    }
    else
    {
        this->SmartOffsetmode = false;
    }
}

// 文件读写

void MainWindow::FileOpenAction()
{
    this->ComboxCheck();
    QString filename = General::SearchFileCSV();
    if(filename == "")
    {
        QMessageBox::warning(nullptr, "警告", "请选择文件!");
        return;
    }
    this->DBGfilename1 = filename;
    if(this->Platform == E_Platform::FVC3){
        this->LaneMkrData = General::ReadCsvFile(General::FileRename(filename,"LaneMark"));
        this->LaneMkrHeader = General::GetCsvHeader(this->LaneMkrData);
        this->VehSelfData = General::ReadCsvFile(General::FileRename(filename,"VehicleData"));
        this->VehSelfHeader = General::GetCsvHeader(this->VehSelfData);
        this->PahHistData = General::ReadCsvFile(General::FileRename(filename,"PahHist"));
        this->PahHistHeader = General::GetCsvHeader(this->PahHistData);
        this->ObjectData = General::ReadCsvFile(General::FileRename(filename,"Object"));
        this->ObjectHeader = General::GetCsvHeader(this->ObjectData);
        this->RoadEdgeData = General::ReadCsvFile(General::FileRename(filename,"RoadEdge"));
        this->RoadEdgeHeader = General::GetCsvHeader(this->RoadEdgeData);
        this->RoadSignData = General::ReadCsvFile(General::FileRename(filename,"RoadSign"));
        this->RoadSignHeader = General::GetCsvHeader(this->RoadSignData);
    }
    else if (this->Platform == E_Platform::FVC2_8){
        this->LaneMkrData = General::ReadCsvFile(General::FileRename(filename,"Lane"));
        this->LaneMkrHeader = General::GetCsvHeader(this->LaneMkrData);
        this->VehSelfData = General::ReadCsvFile(General::FileRename(filename,"VehSelf"));
        this->VehSelfHeader = General::GetCsvHeader(this->VehSelfData);
        this->PahHistData = General::ReadCsvFile(General::FileRename(filename,"PahHist"));
        this->PahHistHeader = General::GetCsvHeader(this->PahHistData);
        this->ObjectData = General::ReadCsvFile(General::FileRename(filename,"Objects"));
        this->ObjectHeader = General::GetCsvHeader(this->ObjectData);
    }
    this->SysInit();
    this->SilderReset();
    QString FileRecord  = General::GetFileRecordTime(filename);
    ui->RecordTime->setText(FileRecord);
    QMessageBox::warning(nullptr,"提示", "新文件载入完成!");
    this->DBGfilename1 = filename;
    this->ReflashResimTextEdit();
}

void MainWindow::DataUpdate(int index)
{
    this->vehself.Update(this->VehSelfData[index],this->VehSelfHeader);
    qDebug()<<"Vehself";
    this->LaneMarker.Update(this->LaneMkrData[index],this->LaneMkrHeader,this->Platform);
    // Object
    qDebug()<<"LaneMarker";
    for(int i = 0;i<this->obj.size();i++)
    {
        this->obj[i].Update(this->ObjectData[index],this->ObjectHeader,i,this->vehself.VLgt,this->Platform);
    }
    qDebug()<<"Object";
    // PahHist
    for (int j = 0; j<this->Path.size(); j++) {
        this->Path[j].Update(this->PahHistData[index],this->PahHistHeader,j,this->Platform);
    }
    qDebug()<<"PahHist";
    this->RoadEdge.Update(this->RoadEdgeData[index],this->RoadEdgeHeader,this->Platform);
    qDebug()<<"RoadEdge";
    this->RoadSigns.Update(this->RoadSignData[index],this->RoadSignHeader,this->Platform);
    qDebug()<<"RoadSigns";
}


// Combox
void MainWindow::ComboxCheck()
{
    if(ui->comboBox->currentText() == "FVC3")
    {
        this->Platform = E_Platform::FVC3;
    }
    else if (ui->comboBox->currentText() == "FVC3.1")
    {
        this->Platform = E_Platform::FVC3_1;
    }
    else if (ui->comboBox->currentText() == "FVC2.8")
    {
        this->Platform = E_Platform::FVC2_8;
    }
    else if (ui->comboBox->currentText() == "FVC2")
    {
        this->Platform = E_Platform::FVC2;
    }
    else
    {
        this->Platform = E_Platform::FVC2_1;
    }
}
//Silder
void MainWindow::SilderReset()
{
    ui->TimehorizontalSlider->setMinimum(0);
    ui->TimehorizontalSlider->setMaximum(this->maxindex);
    ui->TimehorizontalSlider->setValue(0);
}
void  MainWindow::Silderflash()
{
    ui->TimehorizontalSlider->setValue(this->currentindex);
    double recordtime = double(this->currentindex)*0.025;
    QString formattedString = QString("%1:%2").arg(static_cast<int>(recordtime)).arg(static_cast<int>((recordtime - static_cast<int>(recordtime)) * 1000), 3, 10, QChar('0'));
    ui->TimeLabel->setText(formattedString);
}
void MainWindow::SilderReleasedAction()
{
    this->currentindex = ui->TimehorizontalSlider->value();
    this->DataUpdate(this->currentindex);
    this->PlotView();
    this->LCDReflash();
    this->Silderflash();
}
//SYS
void MainWindow::SysInit()
{
    int a = qMin(this->LaneMkrData.size(),this->VehSelfData.size());
    int b = qMin(this->PahHistData.size(),this->ObjectData.size());
    this->maxindex = qMin(a,b);
    this->currentindex = 0;
}
//LCD
void MainWindow::LCDReflash()
{
    ui->Spd_lcdNumber->display(QString::number(this->vehself.VLgt*3.6,'f',1));
    QPointF Point = this->vehself.GetFrontTireDistance2Lane(this->LaneMarker,2.638,1.819);
    ui->LDWL_lcdNumber->display(QString::number(Point.x(),'f',2));
    ui->LDWR_lcdNumber->display(QString::number(Point.y(),'f',2));
    QPointF PointC0 = this->LaneMarker.GetHostLaneC0();
    QPointF PointC1 = this->LaneMarker.GetHostLaneC1();
    QPointF PointC2 = this->LaneMarker.GetHostLaneC2();
    ui->LC0_lcdNumber->display(QString::number(PointC0.x(),'f',2));
    ui->LC1_lcdNumber->display(QString::number(PointC0.y(),'f',3));
    ui->RC0_lcdNumber->display(QString::number(PointC1.x(),'f',2));
    ui->RC1_lcdNumber->display(QString::number(PointC1.y(),'f',3));
    ui->LaneWidth_lcdNumber->display(QString::number(PointC0.x()-PointC0.y(),'f',3));
    ui->LaneSlope_lcdNumber->display(QString::number(0.5*PointC1.x()+0.5*PointC1.x(),'f',3));
    double LaneRadius = qMin(3000.0,qAbs(1/(PointC2.x()+PointC2.y()+0.0001)));
    ui->LaneRadius_lcdNumber->display(QString::number(LaneRadius,'f',0));
    ui->LeftEdgeNumber->display(this->RoadEdge.Left.ConstCoeff);
    ui->RightEdgeNumber->display(this->RoadEdge.Right.ConstCoeff);
}


//plot
void MainWindow::CycleFunction()
{
    this->currentindex += 1;
    if(this->currentindex >= this->maxindex)
    {
        this->SysTimer->stop();
        QMessageBox::warning(nullptr, "警告", "已经到最后!");
        return;
    }
    else
    {
        this->DataUpdate(this->currentindex);
        this->PlotView();
        this->LCDReflash();
        this->Silderflash();
        this->TableViewUpdate();
    }
}
void MainWindow::PlotView()
{
    ui->customplot2->clearGraphs();
    ui->customplot2->clearItems();
    ui->customplot2->clearPlottables();
    this->PlotLane();
    this->PlotRoadEdge();
    this->PlotStopLane();
    this->PlotCrossLane();
    this->PlotObject();
    if (this->PAmode){
        this->PlotPath();
    }
    QCPItemRect* rect = new QCPItemRect(ui->customplot2);
    rect->topLeft->setCoords(-1,3);
    rect->bottomRight->setCoords(1,0);
    ui->customplot2->xAxis->setRange(-8,8);
    ui->customplot2->xAxis->setRangeReversed(true);
    ui->customplot2->yAxis->setRange(-10,150);
    if (this->DCLCmode || this->ALCmode)
    {
        ui->customplot2->xAxis->setRange(-8,8);
        ui->customplot2->xAxis->setRangeReversed(true);
        ui->customplot2->yAxis->setRange(-80,150);
    }

    if (this->Grid){
        ui->customplot2->xAxis->grid()->setVisible(true);
        ui->customplot2->yAxis->grid()->setVisible(true);
    }
    ui->customplot2->setAntialiasedElements(QCP::aeAll);
    ui->customplot2->replot();
}
void MainWindow::PlotLane()
{
    ui->customplot2->addGraph();
    ui->customplot2->graph(0)->setData(this->LaneMarker.GetLaneDataY(E_LanePosition::ClsLe),this->LaneMarker.GetLaneDataX(E_LanePosition::ClsLe),true);
    ui->customplot2->graph(0)->setPen(QPen(Qt::black, 2)); // 设置曲线颜色为黑色，线宽为 2 像素
    ui->customplot2->addGraph();
    ui->customplot2->graph(1)->setData(this->LaneMarker.GetLaneDataY(E_LanePosition::ClsRi),this->LaneMarker.GetLaneDataX(E_LanePosition::ClsRi),true);
    ui->customplot2->graph(1)->setPen(QPen(Qt::black, 2)); // 设置曲线颜色为黑色，线宽为 2 像素
    ui->customplot2->addGraph();
    ui->customplot2->graph(2)->setData(this->LaneMarker.GetLaneDataY(E_LanePosition::SecClsLe),this->LaneMarker.GetLaneDataX(E_LanePosition::SecClsLe),true);
    ui->customplot2->graph(2)->setPen(QPen(Qt::black, 3)); // 设置曲线颜色为黑色，线宽为 2 像素
    ui->customplot2->addGraph();
    ui->customplot2->graph(3)->setData(this->LaneMarker.GetLaneDataY(E_LanePosition::SecClsRi),this->LaneMarker.GetLaneDataX(E_LanePosition::SecClsRi),true);
    ui->customplot2->graph(3)->setPen(QPen(Qt::black, 3)); // 设置曲线颜色为黑色，线宽为 2 像素
}
void MainWindow::PlotPath()
{
    int index  = 6;
    for (int j = 0; j<this->Path.size(); j++) {
        if(this->Path[j].GetTrailState() != 0)
        {
            if (this->Path[j].GetYdata().size()>10)
            {
                ui->customplot2->addGraph();
                ui->customplot2->graph(index)->setData(this->Path[j].GetYdata(),this->Path[j].GetXdata(),true);
                ui->customplot2->graph(index)->setPen(QPen(Qt::red, 2)); // 设置曲线颜色为黑色，线宽为 2 像素
                ui->customplot2->graph(index)->setAdaptiveSampling(false);
                index += 1;
            }
        }
    }
}
void MainWindow::PlotObject()
{
    for(int i = 0;i<this->obj.size();i++)
    {
        if (this->obj[i].isObjectValid())
        {
            QCPItemRect* rect = new QCPItemRect(ui->customplot2);
            QPointF TopLeft = this->obj[i].GetTopLeftPoint();
            QPointF BottomRight = this->obj[i].GetbottomRightPoint();
            rect->topLeft->setCoords(TopLeft.x(),TopLeft.y());
            rect->bottomRight->setCoords(BottomRight.x(),BottomRight.y());
            switch (this->obj[i].PlotColorMode()) {
            case FrontVision_FrontRadar:
                rect->setPen(QPen(Qt::red,2));
                rect->setBrush(QBrush(Qt::blue));
                break;
            case FrontVision_CornerRadar:
                rect->setPen(QPen(Qt::red,2));
                rect->setBrush(QBrush(Qt::green));
                break;
            case FrontVision:
                rect->setPen(QPen(Qt::red,2));
                rect->setBrush(QBrush(Qt::white));
                break;
            case FrontRadar:
                rect->setPen(QPen(Qt::black,2));
                rect->setBrush(QBrush(Qt::blue));
                break;
            case CornerRadar:
                rect->setPen(QPen(Qt::black,2));
                rect->setBrush(QBrush(Qt::green));
                break;
            case None:
                rect->setPen(QPen(Qt::black,2));
                rect->setBrush(QBrush(Qt::green));
                break;
            default:
                rect->setPen(QPen(Qt::black,2));
                rect->setBrush(QBrush(Qt::white));
                break;
            }
            QCPItemText* textItem = new QCPItemText(ui->customplot2);
            QPointF Postion = this->obj[i].GetObjectPos();
            QString str1 = "ID: " + QString::number(this->obj[i].object_ID)+" - "+this->obj[i].GetObjectType();

            textItem->setText(str1); // 设置文本内容
            textItem->position->setCoords(Postion.x(), Postion.y()-5); // 设置文本位置，即文本的中心点坐标
        }
    }
}
void MainWindow::TableViewUpdate()
{
    QStandardItemModel* objtable_model = new QStandardItemModel();
    objtable_model->setColumnCount(10);
    objtable_model->setHeaderData(0,Qt::Horizontal,"ID");
    objtable_model->setHeaderData(1,Qt::Horizontal,"PosLgt");
    objtable_model->setHeaderData(2,Qt::Horizontal,"PosLat");
    objtable_model->setHeaderData(3,Qt::Horizontal,"VLgt");
    objtable_model->setHeaderData(4,Qt::Horizontal,"VLat");
    objtable_model->setHeaderData(5,Qt::Horizontal,"Typ");
    objtable_model->setHeaderData(6,Qt::Horizontal,"TTC");
    objtable_model->setHeaderData(7,Qt::Horizontal,"Dist2LeftLane");
    objtable_model->setHeaderData(8,Qt::Horizontal,"Dist2RightLane");
    objtable_model->setHeaderData(9,Qt::Horizontal,"Dist2Ego");
    int index = 0;
    for(int i = 0;i<this->obj.size();i++)
    {
        if (this->obj[i].isObjectValid())
        {
            QVector<QString> Message = this->obj[i].GetObjectInfo(this->LaneMarker);
            objtable_model->setItem(index,0,new QStandardItem(Message[0]));
            objtable_model->setItem(index,1,new QStandardItem(Message[1]));
            objtable_model->setItem(index,2,new QStandardItem(Message[2]));
            objtable_model->setItem(index,3,new QStandardItem(Message[3]));
            objtable_model->setItem(index,4,new QStandardItem(Message[4]));
            objtable_model->setItem(index,5,new QStandardItem(Message[5]));
            objtable_model->setItem(index,6,new QStandardItem(Message[6]));
            objtable_model->setItem(index,7,new QStandardItem(Message[7]));
            objtable_model->setItem(index,8,new QStandardItem(Message[8]));
            objtable_model->setItem(index,9,new QStandardItem(Message[9]));
            index++;
        }
    }
    ui->tableView->setModel(objtable_model);
}
void MainWindow::PlotRoadEdge()
{
    ui->customplot2->addGraph();
    ui->customplot2->graph(4)->setData(this->RoadEdge.GetRoadEdgeDataY(E_LanePosition::ClsLe),this->RoadEdge.GetRoadEdgeDataX(E_LanePosition::ClsLe),true);
    ui->customplot2->graph(4)->setPen(QPen(Qt::red, 4)); // 设置曲线颜色为黑色，线宽为 2 像素
    ui->customplot2->addGraph();
    ui->customplot2->graph(5)->setData(this->RoadEdge.GetRoadEdgeDataY(E_LanePosition::ClsRi),this->RoadEdge.GetRoadEdgeDataX(E_LanePosition::ClsRi),true);
    ui->customplot2->graph(5)->setPen(QPen(Qt::red, 4)); // 设置曲线颜色为黑色，线宽为 2 像素
}
void MainWindow::PlotStopLane()
{
    for(int i = 0;i<this->RoadSigns.SL.size();i++)
    {
        if(this->RoadSigns.SL[i].IsValid())
        {
            QCPItemRect* rect = new QCPItemRect(ui->customplot2);
            QPointF TopLeft = this->RoadSigns.SL[i].GetTopLeftPoint();
            QPointF BottomRight = this->RoadSigns.SL[i].GetbottomRightPoint();
            rect->topLeft->setCoords(TopLeft.x(),TopLeft.y());
            rect->bottomRight->setCoords(BottomRight.x(),BottomRight.y());
            rect->setPen(QPen(Qt::red,2));
            rect->setBrush(QBrush(Qt::red));
        }
    }
}

void MainWindow::PlotCrossLane()
{
    for(int i = 0;i<this->RoadSigns.CL.size();i++)
    {
        if(this->RoadSigns.CL[i].IsValid())
        {
            QCPItemRect* rect = new QCPItemRect(ui->customplot2);
            QPointF TopLeft = this->RoadSigns.CL[i].GetTopLeftPoint();
            QPointF BottomRight = this->RoadSigns.CL[i].GetbottomRightPoint();
            rect->topLeft->setCoords(TopLeft.x(),TopLeft.y());
            rect->bottomRight->setCoords(BottomRight.x(),BottomRight.y());
            rect->setPen(QPen(Qt::cyan,2));
            rect->setBrush(QBrush(Qt::cyan));
        }
    }
}

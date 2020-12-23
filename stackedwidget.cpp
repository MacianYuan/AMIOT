/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :stackedwidget.cpp
* @Platform     :Hi3520D - linux-3.0.8 - Qt4.8.6 - arm-hisiv100nptl-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************29-June-2020(MacianYuan):
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.  2010-2020  All rights reserved.
******************************************************************************************/

#include "stackedwidget.h"
#include "ui_stackedwidget.h"
#include <QDebug>
#include <QToolButton>

stackedwidget::stackedwidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::stackedwidget)
{
    ui->setupUi(this);
    //地图矢量图和卫星图切换按钮
    ui->hybridButton->setChecked(false);

    //定时更新GPS mark 经纬度、stackedwidget 内容信息
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(mapSlotTimeout()));
    timer->start(1000);

    //在线地图
    onlineMapScene = new QGraphicsScene();
    onlineMapWebView = new QWebView();
    connect(onlineMapWebView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(mapSlotPopulateJavaScriptWindowObject()));
    onlineMapWebView->setUrl(QUrl("qrc:/Tmap/map.html"));
    onlineMapWebView->setContentsMargins(0,0,0,0);
    onlineMapWebView->setTextSizeMultiplier(1);//设置网页字体大小

    //单singleShot，只会触发一次，发出一次信号，然后来执行 初始化地图信息 槽函数
    QTimer::singleShot(60, this, SLOT(mapInitialization()));
}



//定时更新GPS mark 经纬度
void stackedwidget::mapSlotTimeout()
{
    //更新地图GPS Mark信息
   // static double tempLon=117.146163;
    //tempLon+=0.0001;
    static unsigned int process_count ;
    if(process_count == 5){
        get_disk_space();
    }
    if(process_count == 10){
        process_count = 0;
    }
    mapSetCoordinate(QString::number(Device_public::gps_information.GPS_Longitude+0.05936852),QString::number(Device_public::gps_information.GPS_Latitude+0.33128946));


    //刷新页面状态显示
    QFont ft;
    ft.setPointSize(24);
    //获取到机具号显示机具号，获取不到显示正在获取机具号...
    if(!Device_public::device_number.isEmpty()){
        ui->label_device_number->setText(Device_public::device_number);
    }else{
        ui->label_device_number->setText("正在获取机具号...");
    }
    if(Device_public::Motype <= 0){
        ui->label_system_id->setText("正在获取工作模式...");
    }
    ui->label_device_number->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_device_number->setFont(ft);
    // ui->label_device_number->move(450,60);
    //作业模式
    switch (Device_public::Motype) {
    case 1:
        ui->label_system_id->setText("深松作业");
        break;
    case 2:
        ui->label_system_id->setText("环田作业");
        break;
    case 3:
        ui->label_system_id->setText("深翻作业");
        break;
    case 4:
        ui->label_system_id->setText("免耕播种");
        break;
    case 5:
        ui->label_system_id->setText("稻麦收获");
        break;
    case 6:
        ui->label_system_id->setText("玉米收获");
        break;
    case 7:
        ui->label_system_id->setText("位置服务");
        break;
    case 8:
        ui->label_system_id->setText("打捆作业");
        break;
    case 9:
        ui->label_system_id->setText("中耕作业");
        break;
    case 10:
        ui->label_system_id->setText("植保作业");
        break;
    case 11:
        ui->label_system_id->setText("插秧作业");
        break;
    case 12:
        ui->label_system_id->setText("旋耕作业");
        break;
    case 13:
        ui->label_system_id->setText("抛肥作业");
        break;
    case 14:
        ui->label_system_id->setText("收获作业");
        break;
    case 15:
        ui->label_system_id->setText("中耙作业");
        break;
    default:
        break;
    }
    ui->label_system_id->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_system_id->setFont(ft);
    //运行状态
    ui->label_running_status->setText("正常");
    ui->label_running_status->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_running_status->setFont(ft);
    //传感器状态

    if(Device_public::state_Sersor == 0){
        ui->label_sensor_status->setText("传感器检测中...");
    }else if(Device_public::state_Sersor == 1){
        ui->label_sensor_status->setText("正常");
    }else if(Device_public::state_Sersor == 2){
        ui->label_sensor_status->setText("脱机");
    }
    ui->label_sensor_status->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_sensor_status->setFont(ft);
    //摄像头状态
    ui->label_camera_status->setText("正常");
    ui->label_camera_status->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_camera_status->setFont(ft);
    //TF卡状态
    ui->label_tf_status->setText("正常");
    ui->label_tf_status->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_tf_status->setFont(ft);
    //定位状态
    if(Device_public::state_GPS == false){
        ui->label_positioning_status->setText("GPS正在定位");
    }else{
        ui->label_positioning_status->setText("GPS定位成功");
    }
    ui->label_positioning_status->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_positioning_status->setFont(ft);
    //系统启动
    ui->label_system_booting->setText("正常");
    ui->label_system_booting->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_system_booting->setFont(ft);

    //作业深度
    ui->label_depth->setText(" ");
    ui->label_depth->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_depth->setFont(ft);
    //经纬度坐标
    QString x_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Longitude)
            .arg(Device_public::gps_information.GPS_Longitude_direction);


    QString y_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Latitude)
            .arg(Device_public::gps_information.GPS_Latitude_direction);

    ui->label_x_data->setText(x_data);
    ui->label_x_data->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_x_data->setFont(ft);

    ui->label_y_data->setText(y_data);
    ui->label_y_data->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_y_data->setFont(ft);

    //运行状态
    ui->label_running_status2->setText("正常");
    ui->label_running_status2->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_running_status2->setFont(ft);

    //当前油量
    ui->label_current_oil->setText(QString("%1%").arg(Device_public::sensor_oil_data));
    ui->label_current_oil->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_current_oil->setFont(ft);

    //当前预计面积
    ui->label_expected_area->setText(" ");
    ui->label_expected_area->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_expected_area->setFont(ft);
    //TF卡存储

    QString use = QString::number(Device_public::state_tf_use,'f',0);
    QString total = QString::number(Device_public::state_tf_total,'f',2);
    ui->label_tf_storage->setText(QString("%1M(%2G)").arg(use).arg(total));
    ui->label_tf_storage->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_tf_storage->setFont(ft);
    //当天累计面积
    ui->label_tota_area->setText(" ");
    ui->label_tota_area->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_tota_area->setFont(ft);

    process_count ++;
}
bool stackedwidget::get_disk_space()
{
    QProcess process;
    process.start("df -k");
    process.waitForFinished();
    process.readLine();
    while(!process.atEnd())
    {
        QString str = process.readLine();
        if(str.startsWith("/dev/sda1"))
        {
            str.replace("\n","");
            str.replace(QRegExp("( ){1,}")," ");
            auto lst = str.split(" ");
            if(lst.size() > 5){
                   Device_public::state_tf_use = lst[2].toDouble()/1024.0;
                   Device_public::state_tf_total = lst[1].toDouble()/1024.0/1024.0;
            }
                qDebug("mount:%s use:%.0lfMB total:%.0lfMB",lst[5].toStdString().c_str(),lst[2].toDouble()/1024.0,lst[1].toDouble()/1024.0/1024.0);
        }
    }
    return true;
}
//向地图传递类信息
void stackedwidget::mapSlotPopulateJavaScriptWindowObject()
{
    onlineMapWebView->page()->mainFrame()->addToJavaScriptWindowObject(QString("TMap"),this);
}

//卫星图与矢量图切换
void stackedwidget::on_hybridButton_clicked()
{
    qDebug() << "1";
    QString cmd;
    QWebFrame *webFrame = onlineMapWebView->page()->mainFrame();
    if(ui->hybridButton->isChecked()){
        cmd = QString("hybrid()");
        qDebug() << "2";
        ui->hybridButton->setChecked(true);
        //ui->hybridButton->setStyleSheet("image: url(:/image/image/1_03.png);");
    }
    else{
        cmd = QString("vector()");
        qDebug() << "3";
        ui->hybridButton->setChecked(false);
        //ui->hybridButton->setStyleSheet("image: url(:/image/image/1_03.png);");
    }
    webFrame->evaluateJavaScript(cmd);
}

////摄像头页面 定时器触发后标题隐藏
//void stackedwidget::on_camera_hide()
//{
//    ui->frame_biaoti3cammeraDisplay->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
//}

//初始化地图信息
void stackedwidget::mapInitialization()
{
    onlineMapWebView->setGeometry(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    onlineMapWebView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    onlineMapWebView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOff);
    onlineMapScene->addWidget(onlineMapWebView);
    ui->graphicsView->setScene(onlineMapScene);
}


//设置GPS mark 经纬度
void stackedwidget::mapSetCoordinate(QString lon,QString lat)
{
    //qDebug()<< lon << lat;
    QWebFrame *webFrame = onlineMapWebView->page()->mainFrame();
    QString cmd = QString("showAddress(\"%1\",\"%2\")").arg(lon).arg(lat);
    webFrame->evaluateJavaScript(cmd);
}

stackedwidget::~stackedwidget()
{
    delete ui;
}

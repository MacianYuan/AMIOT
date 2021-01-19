/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :mainwindow.cpp
* @Platform     :Hi3520D - linux-3.0.8 - Qt4.8.6 - arm-hisiv100nptl-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************29-June-2020(MacianYuan):增加串口1 和mqtt功能
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.  2010-2020  All rights reserved.
******************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

/******************************************************************************************
* @Function         :MainWindow(QWidget *parent)
*
* @Purpose          :构造函数
* @Description      :
*********************系统初始化:串口初始化,系统时间显示和刷新,接收数据处理,
*********************界面ui显示:
*********************
* @Grammar&Demand   :<QWSServer>
*********************
*********************
* @Param            :
* @Retval           :
* @Others           :
* @e.g.             :
******************************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    //鼠标箭头隐藏
//    QWSServer::setCursorVisible(FALSE);

    //全屏显示，设定窗口透明显示
    //showFullScreen();
//    QWSServer::setBackground(QColor(0,0,0,0));
    setAttribute(Qt::WA_TranslucentBackground);
    //显示内容页
    stackedwidget_I = new stackedwidget(this);
    stackedwidget_I->setGeometry(0,50,800,550);
    page_id = 4;
    stackedwidget_I->setCurrentIndex(page_id);
    stackedwidget_I->show();

    //显示菜单栏
    menuwidget_I = new menuwidget(this);
    menuwidget_I->setGeometry(0,50,160,280);
    menuwidget_I->show();


    //触屏、鼠标按键点击链接信号槽   (内容页 page Index) 和  (菜单栏button ID)
    //connect(menuwidget_I, SIGNAL(signal_buttonClicked(int)),this, SLOT(on_buttonClickedLeftNav(int)));

    //切换页面 按钮选中相应背景图切换
    connect(this,SIGNAL(menushowSelect()),menuwidget_I,SLOT(on_menushowSelect()));
    connect(this,SIGNAL(menubuttonshow(int)),menuwidget_I,SLOT(on_menubuttonshow(int)));
    //摄像头界面隐藏mainwidow的相关插件 整个window表头背景，时间显示
    connect(menuwidget_I,SIGNAL(camera_hide()),this,SLOT(on_camera_hide()));
    //摄像头界面隐藏stackedwidget的相关插件 摄像头widget表头背景隐藏
    //connect(menuwidget_I,SIGNAL(camera_hide()),stackedwidget_I,SLOT(on_camera_hide()));
}


void MainWindow::on_menushowSelect()
{   //如果当前显示是视频监视页  且  菜单栏已经显示   菜单隐藏功能取消
    if((stackedwidget_I->currentIndex() == 2) && (menuwidget_I->isVisible() == false))
        return;
    emit menushowSelect();
}

void MainWindow::on_label_time_refresh(QString str)
{
    ui->label_time->setText(str);
}

void MainWindow::on_gps_signal_update()
{
    //qDebug()<<"Device_public::gps_information.GPS_num_satellites"<<Device_public::gps_information.GPS_num_satellites;
    if( Device_public::gps_information.GPS_num_satellites < 3 ){
        ui->label_gps->setStyleSheet("border-image: url(:/gps_1.png);");
    }else if( ( Device_public::gps_information.GPS_num_satellites >= 3 ) && ( Device_public::gps_information.GPS_num_satellites < 6 )){
        ui->label_gps->setStyleSheet("border-image: url(:/gps_2.png);");
    }else if( ( Device_public::gps_information.GPS_num_satellites >= 6 ) && ( Device_public::gps_information.GPS_num_satellites < 9 )){
        ui->label_gps->setStyleSheet("border-image: url(:/gps_3.png);");
    }else if( Device_public::gps_information.GPS_num_satellites >= 9 ){
        ui->label_gps->setStyleSheet("border-image: url(:/gps_4.png);");
    }
}

//红外检测到上下键，stackedwidget翻页
void MainWindow::on_stackedshowSelect(bool page_flag)
{
    //mebuwidget button显示才执行
    if(menuwidget_I->menu_button_show == 1){
        //printf("on_stackedshowSelect");
        //上键触发 页面index --
        if(page_flag == true){
            page_id--;
            if(page_id == -1)
                page_id = 4;
        }
        //下键触发 页面index ++
        if(page_flag == false){
            page_id++;
            if(page_id == 5)
                page_id = 0;
        }
        //改变menuwidget 内button的StyleSheet 背景图片  以及定时菜单隐藏和标题框隐藏
        emit menubuttonshow(page_id);
        //页面切换
        stackedwidget_I->setCurrentIndex(page_id);
        if(page_id != 2){
            ui->label_background->setStyleSheet("background-image: url(:/shang_01.bmp);");
            ui->label_time->show();
        }
    }
}

void MainWindow::on_stream_change(bool change_flag)
{
    if(page_id == 2){
        if( change_flag == true){
            slotLayout1Action();
            stackedwidget_I->show();
        }
        if( change_flag == false){
            slotLayout4Action();
            stackedwidget_I->show();
        }
    }
}

//触屏、鼠标按键点击槽函数(内容页 page Index)
void MainWindow::on_buttonClickedLeftNav(int id)
{
    stackedwidget_I->setCurrentIndex(id);
}

void MainWindow::slotLayout1Action()
{
    qDebug()<<"Mal_VO_SetLayout(1)";
 //   Mal_VO_SetLayout(1);
}

void MainWindow::slotLayout4Action()
{
    qDebug()<<"Mal_VO_SetLayout(4)";
//    Mal_VO_SetLayout(4);
}

void MainWindow::slotLayout9Action()
{
 //   Mal_VO_SetLayout(9);
}


//主页面是camera页面时，定时将标题栏和时间
void MainWindow::on_camera_hide()
{
    ui->label_background->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->label_time->hide();
    //stackedwidget_I->setCurrentIndex(2);//重新刷新视频显示页面
}

MainWindow::~MainWindow()
{
    delete ui;
}



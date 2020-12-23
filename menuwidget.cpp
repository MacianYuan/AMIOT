/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :menuwidget.cpp
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

#include "menuwidget.h"
#include "ui_menuwidget.h"

menuwidget::menuwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menuwidget)
{
    ui->setupUi(this);
    //初始化 按键组
    this->initButtonToGroup();
    camera_timer = new QTimer(this);
    connect(camera_timer,SIGNAL(timeout()),this,SLOT(cameraSlotTimeout()));

    //菜单按键翻转显示
    connect(ui->toolButton_menu,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_toggled(bool)));
    //将私有的按键组ID通过信号发送
    connect(ui->buttonGroup_left_nav, SIGNAL(buttonClicked(QAbstractButton *)),
            this, SLOT(slot_buttonClicked(QAbstractButton *)));

}

//摄像头界面 菜单栏 和标题栏隐藏功能
void menuwidget::cameraSlotTimeout()
{
    camera_timer->stop();
    //qDebug()<<"cameraSlotTimeout1";
    ui->toolButton_systemState->hide();
    ui->toolButton_workingState->hide();
    ui->toolButton_cameraDisplay->hide();
    ui->toolButton_mapNavigation->hide();
    //ui->toolButton_systemSetup->hide();
    ui->toolButton_startDisplay->hide();
    ui->toolButton_menu->setStyleSheet("border-image: url(:/images/caidan_01.bmp);");
    menu_button_show = 0;
    pushbutton_toggle = true;
    emit camera_hide();
}

//菜单button 选中状态显示
void menuwidget::on_menubuttonshow(int page)
{
    ui->toolButton_systemState->setStyleSheet("border-image: url(:/images/xiala_01.png);");
    ui->toolButton_workingState->setStyleSheet("border-image: url(:/images/xiala_02.png);");
    ui->toolButton_cameraDisplay->setStyleSheet("border-image: url(:/images/xiala_03.png);");
    ui->toolButton_mapNavigation->setStyleSheet("border-image: url(:/images/xiala_04.png);");
    ui->toolButton_startDisplay->setStyleSheet("border-image: url(:/images/xiala_06.png);");
    //根据当前stackwidget page index 判断出相应的button背景图片的更换
    switch (page){
        case 0:{
            camera_timer->stop();
            ui->toolButton_systemState->setStyleSheet("border-image: url(:/images/xiala_01_02.png);");
        }break;
        case 1:{
            camera_timer->stop();
            ui->toolButton_workingState->setStyleSheet("border-image: url(:/images/xiala_02_02.png);");
        }break;
        case 2:{
            //qDebug()<<"cameraSlotTimeout";
            //计时开始 按键菜单隐藏  标题框隐藏
            camera_timer->start(1000);
            ui->toolButton_cameraDisplay->setStyleSheet("border-image: url(:/images/xiala_03_02.png);");
        }break;
        case 3:{
            camera_timer->stop();
            ui->toolButton_mapNavigation->setStyleSheet("border-image: url(:/images/xiala_04_02.png);");
        }break;
        case 4:{
            camera_timer->stop();
            ui->toolButton_startDisplay->setStyleSheet("border-image: url(:/images/xiala_06_02.png);");
        }break;
        default:{
            camera_timer->stop();
        }break;
    }
}



//根据按键选择标志位，进行菜单栏按键显示和隐藏的实现
void menuwidget::on_menushowSelect()
{
    switch(pushbutton_toggle){
        //隐藏 置true   判断为true  即为可显示
        case true:{
            //printf("pushButton clicked 1\n");
            ui->toolButton_systemState->show();
            ui->toolButton_workingState->show();
            ui->toolButton_cameraDisplay->show();
            ui->toolButton_mapNavigation->show();
            // ui->toolButton_systemSetup->show();
            ui->toolButton_startDisplay->show();
            pushbutton_toggle = false;
            menu_button_show = 1;
            ui->toolButton_menu->setStyleSheet("border-image: url(:/images/caidan_02.bmp);");
        }break;

        //显示 置false  判断为false 即为可隐藏
        case false:{
            //printf("pushButton clicked 2\n");
            ui->toolButton_systemState->hide();
            ui->toolButton_workingState->hide();
            ui->toolButton_cameraDisplay->hide();
            ui->toolButton_mapNavigation->hide();
            //ui->toolButton_systemSetup->hide();
            ui->toolButton_startDisplay->hide();
            pushbutton_toggle = true;
            menu_button_show = 0;
            ui->toolButton_menu->setStyleSheet("border-image: url(:/images/caidan_01.bmp);");
        }break;
        default:{

        }break;
    }
}



//将私有的按键组ID通过信号发送
void menuwidget::slot_buttonClicked(QAbstractButton *btn)
{
    int id = ui->buttonGroup_left_nav->id(btn);
    //qDebug()<< id;
    //将私有的按键组ID通过信号发送
    emit signal_buttonClicked(id);

    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}


//初始化 按键组
void menuwidget::initButtonToGroup()
{
    ui->buttonGroup_left_nav->setId(ui->toolButton_systemState, 0);
    ui->buttonGroup_left_nav->setId(ui->toolButton_workingState, 1);
    ui->buttonGroup_left_nav->setId(ui->toolButton_cameraDisplay, 2);
    ui->buttonGroup_left_nav->setId(ui->toolButton_mapNavigation, 3);
   // ui->buttonGroup_left_nav->setId(ui->toolButton_systemSetup, 4);
    ui->buttonGroup_left_nav->setId(ui->toolButton_startDisplay, 4);
    //菜单按键翻转显示  默认隐藏
    pushbutton_toggle = true;
    ui->toolButton_systemState->hide();
    ui->toolButton_workingState->hide();
    ui->toolButton_cameraDisplay->hide();
    ui->toolButton_mapNavigation->hide();
    //ui->toolButton_systemSetup->hide();
    ui->toolButton_startDisplay->hide();
    menu_button_show = 0;
}

//菜单按键翻转显示
void menuwidget::on_pushButton_toggled(bool)
{
    switch(pushbutton_toggle){
        case true:
            //printf("pushButton clicked 1\n");
            ui->toolButton_systemState->show();
            ui->toolButton_workingState->show();
            ui->toolButton_cameraDisplay->show();
            ui->toolButton_mapNavigation->show();
           // ui->toolButton_systemSetup->show();
            ui->toolButton_startDisplay->show();
            pushbutton_toggle = false;
            menu_button_show = 1;
            break;
        case false:
            //printf("pushButton clicked 2\n");
            ui->toolButton_systemState->hide();
            ui->toolButton_workingState->hide();
            ui->toolButton_cameraDisplay->hide();
            ui->toolButton_mapNavigation->hide();
           // ui->toolButton_systemSetup->hide();
            ui->toolButton_startDisplay->hide();
            pushbutton_toggle = true;
            menu_button_show = 0;
            break;
        default:break;
    }
}

menuwidget::~menuwidget()
{
    delete ui;
}

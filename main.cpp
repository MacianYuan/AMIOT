/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :main.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>

#include "mainwindow.h"
#include "mainservice.h"
//#include "sqlworker/sqlworker.h"
#include "hisiservice.h"

int main(int argc, char *argv[])
{
    HisiService *m_phisiservice = new HisiService;
    m_phisiservice->start();
    sleep(5);
    //字体设置
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    //主窗口显示
    MainWindow *mainwindow = new MainWindow();
    mainwindow->showFullScreen();

    //主服务运行
    MainService *mainservice = new MainService;
    mainservice->init();

    //系统逻辑与显示更新
    QObject::connect(mainservice,SIGNAL(menushowSelect()),mainwindow,SLOT(on_menushowSelect()));
    QObject::connect(mainservice,SIGNAL(stackedshowSelect(bool)),mainwindow,SLOT(on_stackedshowSelect(bool)));
    QObject::connect(mainservice,SIGNAL(stream_change(bool)),mainwindow,SLOT(on_stream_change(bool)));
    QObject::connect(mainservice,SIGNAL(gps_signal_update()),mainwindow,SLOT(on_gps_signal_update()));
    QObject::connect(mainservice,SIGNAL(label_time_refresh(QString)),mainwindow,SLOT(on_label_time_refresh(QString)));



    a.exec();
}


#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
//#include <QWebFrame>
//#include <QWebView>
#include <QStackedWidget>
#include <QMessageBox>
#include <QProcess>
#include "device_public.h"

namespace Ui {
class stackedwidget;
}

class stackedwidget : public QStackedWidget
{
    Q_OBJECT
    
public:
    explicit stackedwidget(QWidget *parent = 0);

    ~stackedwidget();    

private:
    Ui::stackedwidget *ui;
    //设置GPS mark 经纬度
    void mapSetCoordinate(QString lon,QString lat);
    bool get_disk_space();
public slots:
    //初始化地图信息
    void mapInitialization();
    //摄像头页面 定时器触发后标题隐藏
    //void on_camera_hide();

private slots:
    //向地图传递类信息
    void mapSlotPopulateJavaScriptWindowObject();
    //定时更新GPS mark 经纬度
    void mapSlotTimeout();
    //卫星图与矢量图切换
    void on_hybridButton_clicked();

private:

    QGraphicsScene			*onlineMapScene;
//    QWebView				*onlineMapWebView;
    QString					mapInformationLst;
    QTimer *timer;

};

#endif // STACKEDWIDGET_H

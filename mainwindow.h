#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QToolButton>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
//#include <QWSServer>
#include <QMenu>
#include <QAction>
//#include <QtGui/QButtonGroup>
#include <QThread>

#include "stackedwidget.h"
#include "menuwidget.h"
#include "device_public.h"
#include "serialcommunication.h"
#include "irdaanalysis.h"
//#include "rtc/hi_rtc.h"
//#include "rtc/rtc_test.h"
#include "httpgetfile.h"
#include "texttospeech.h"
#include "gpsanalysis.h"
#include "sensoranalysis.h"
#include "httpservice.h"

namespace Ui {
class MainWindow;
}

class QMenu;
class QAction;
class QActionGroup;
class MainWindow : public QWidget
{
    Q_OBJECT  
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //切换页面 按钮选中相应背景图切换 对应menuwidget槽函数
    void menubuttonshow(int page);
    void tts_message(unsigned int num,QString str);
    void menushowSelect();

private:

    void slotLayout1Action();
    void slotLayout4Action();
    void slotLayout9Action();

public slots:

    //实时更新界面上时间的值
    void on_label_time_refresh(QString str);

    //红外检测到上下键，stackedwidget翻页
    void on_stackedshowSelect(bool page_flag);
    //主页面是camera页面时，定时将标题栏和时间隐藏
    void on_camera_hide();
    //触屏、鼠标按键点击槽函数(内容页 page Index)
    void on_buttonClickedLeftNav(int id);

    void on_stream_change(bool change_flag);
    void on_menushowSelect();

    void on_gps_signal_update();
private:
    Ui::MainWindow *ui;
    //系统时间显示和刷新
    //串口接收数据处理
    QTimer* RecvRefreshTimer;

    menuwidget *menuwidget_I;
    //显示内容页widget
    stackedwidget * stackedwidget_I;


    //stackedwidget 页面index
    int page_id;
    //获取系统现在的时间
    QDateTime time;

};



#endif // MAINWINDOW_H

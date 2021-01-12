#ifndef MENUWIDGET_H
#define MENUWIDGET_H
#include <QTimer>
#include <QWidget>
#include <QAbstractButton>
#include <QDebug>

#include "stackedwidget.h"
#include "device_public.h"

namespace Ui {
class menuwidget;
}

class menuwidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit menuwidget(QWidget *parent = 0);
    ~menuwidget();

signals:
    //将私有的按键组ID通过信号发送
    void signal_buttonClicked(int id);
    void camera_hide();

private:
    Ui::menuwidget *ui;
    //菜单按键翻转显示
    bool pushbutton_toggle;
    //初始化 按键组
    void initButtonToGroup();
    //进入摄像头页面开始启动定时器，时间到隐藏
    QTimer *camera_timer;

public slots:
    //根据按键选择标志位，进行菜单栏按键显示和隐藏的实现
    void on_menushowSelect();
    //菜单button 选中状态显示
    void on_menubuttonshow(int page);
    //摄像头界面 菜单栏 和标题栏隐藏功能
    void cameraSlotTimeout();
    //菜单按键翻转显示
    void on_pushButton_toggled(bool);
    //将私有的按键组ID通过信号发送
    void slot_buttonClicked(QAbstractButton *btn);

public:
    //mebuwidget button显示标志 ，此标志为true  mainwindow 中检测到按键才执行
    bool menu_button_show;
};

#endif // MENUWIDGET_H

#ifndef IRDAANALYSIS_H
#define IRDAANALYSIS_H
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include <QObject>
#include <QTimer>
#include <QThread>

#include "menuwidget.h"
#include "hiir.h"
#include "hiir_codedef.h"
#include "public/device_public.h"

class IrdaAnalysis : public QObject
{
  Q_OBJECT
public:
    explicit IrdaAnalysis(QObject *parent = 0);
    //红外键盘编码配置 NEC with full repeat code
    static inline void ir_config_fun(int filp, hiir_dev_param dev_param);
    //按键编码解析 通过信号槽 链接到其他类中实现控制
    void normal_report_irkey(irkey_info_s rcv_irkey_info);
    ~IrdaAnalysis();

private:
   //红外键盘解码初始化
   int irda_init();

protected:
    void run();

private:
    //文件描述符
    int m_fd;
    //监听套接字
    QSocketNotifier *m_notifier;

private slots:
     //红外键盘编码配置 NEC with full repeat code
     void on_irda_read();

signals:
     //菜单栏显示 隐藏信号  通过mainwindow 转到menuwidget 槽函数
     void menushowSelect();
     //切换 stackedwidget 页面显示 通过mainwindow 转到mainwindow 槽函数
     void stackedshowSelect(bool);

     void stream_change(bool);
};

#endif // IRDAANALYSIS_H

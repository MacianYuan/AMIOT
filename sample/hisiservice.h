#ifndef HISISERVICE_H
#define HISISERVICE_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include "hisivio.h"


class HisiService : public QThread {
public:
    /**
    * @brief  构造函数。
    * @return 无
    */
    HisiService();
    /**
    * @brief  析构造函数。
    * @return 无
    */
    ~HisiService();
    /**
    * @brief  线程主循环函数。
    * @return 无
    */
    virtual void run();
    /**
    * @brief 线程是否正在运行
    * @return true:正在运行, false:线程已终止.
    */
    bool isWorking();
    /**
    * @brief	终止线程运行
    * @return   void
    */
    void exitThread();
private:
    QMutex m_mtxRunning;/// 保护m_bWorking
    bool m_bWorking;	/// 线程正在工作
    bool m_bFinished;	/// 线程已停止工作
    int enVoMode;
    int enPreVoMode;

public slots:
    void hisiSelectMode(char ch);

};



#endif // HISISERVICE_H

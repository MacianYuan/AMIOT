/******************************************************************************************
*-----First Created. time: 12-Jan-2021	10:30  A.m.
* @File name    :hisivio.c
* @Platform     :Hi3520DV300 - linux-3.0.8 - Qt5.12.7 - arm-hisiv300-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************12-Jan-2021(MacianYuan):
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.   All rights reserved.
******************************************************************************************/
#include "hisiservice.h"


HisiService::HisiService() : QThread(), m_bWorking(false), m_bFinished(false) {   
    enVoMode = 1;
    enPreVoMode = 1;
}


bool HisiService::isWorking() {
    QMutexLocker locker(&m_mtxRunning);
    return m_bWorking;
}

void HisiService::run() {

    m_bWorking = true;

    while (isWorking()) {       
        C_Hisi_VIO_PAL_Init(enVoMode);  //4通道
        sleep(1);
    }
    m_bFinished = true;
}

/*
0 : 1通道
1 : 4通道
*/
void HisiService::hisiSelectMode(char ch_Mode)
{
    enVoMode = ch_Mode;

    if(enPreVoMode != enVoMode){
        enPreVoMode = enVoMode;
        C_Hisi_Select_Chmode(enPreVoMode,enVoMode);
    }

}

void HisiService::exitThread() {
    {
        QMutexLocker locker(&m_mtxRunning);
        m_bWorking = false;
    }
    while (!m_bFinished) {
        QThread::msleep(10);
    }
}

HisiService::~HisiService() {
}

//ns_train::CIniConfig cfg;
//QString strFileName = ns_train::getPath(AMIOT_CONFIG);
//        cfg.setFileName(VIDEO_CONFIG);
//        enVoMode = cfg.getInteger("HisiVideo", "enVoMode", 0);

//        if(enVoMode != enPreVoMode){
//            enPreVoMode = enVoMode;
//            select_ch(enVoMode);
//        }else{


//        }

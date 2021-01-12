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
#ifndef HISIVIO_H
#define HISIVIO_H


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "sample_comm.h"

#include <linux/fb.h>
#include "hifb.h"
//#include "loadbmp.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"


extern int C_Hisi_VIO_PAL_Init(int ch_mode);
extern int C_Hisi_Select_Chmode(int ch_PreMode,int ch_Mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // HISIVIO_H

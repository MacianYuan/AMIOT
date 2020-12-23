/**
* \file mal.c
* \author WooShang <wooshang@126.com>
* \date 2015/07/09
*
* This file interfaces of Middle layout.
* Copyright (C) 2015 WooShang.
*
*/

#include <stdio.h>
#include <string.h>
#include <mpi_sys.h>
#include <mpi_vb.h>
#include "mal.h"
#include "gui.h"
#include "sample/common/sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static SAMPLE_VO_MODE_E enVoMode;
static SAMPLE_VO_MODE_E enPreVoMode = VO_MODE_4MUX;

static int sys_init()
{
   HI_MPI_VB_Exit();
   HI_MPI_SYS_Exit();

   VB_CONF_S stVbConf;
   memset(&stVbConf,0,sizeof(VB_CONF_S));
   stVbConf.u32MaxPoolCnt = 128;
   stVbConf.astCommPool[0].u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_ENCODING_MODE_PAL,PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
   stVbConf.astCommPool[0].u32BlkCnt = 64;

   HI_S32 s32Ret = HI_MPI_VB_SetConf(&stVbConf);
   if (HI_SUCCESS != s32Ret) {
       printf("HI_MPI_VB_SetConf failed=0x%x\n", s32Ret);
       return -1;
   }

   s32Ret = HI_MPI_VB_Init();
   if (HI_SUCCESS != s32Ret) {
       printf("HI_MPI_VB_Init failed!\n");
       return -1;
   }

   MPP_SYS_CONF_S stSysConf = {0};
   stSysConf.u32AlignWidth = 64;//16
   s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
   if(s32Ret != HI_SUCCESS) {
       printf("sys_conf err= %d\n",s32Ret);
       return -1;
   }
   s32Ret = HI_MPI_SYS_Init();
   if(s32Ret != HI_SUCCESS) {
       printf("sys_conf err= %d\n",s32Ret);
       return -1;
   }
   return 0;

}
static int sys_exit()
{
   HI_MPI_VB_Exit();
   HI_MPI_SYS_Exit();
   return 0;
}

int Mal_SYS_Init()
{
   if(sys_init() !=0)
       return -1;

   if(gui_init()!=0)
       return -1;

   return 0;
}

int Mal_SYS_Exit()
{
   gui_exit();
   sys_exit();
   return 0;
}


int Mal_VO_SetLayout(int layout)
{
    HI_S32 s32Ret;
    enVoMode = VO_MODE_1MUX;
    if (1 == layout)
        enVoMode = VO_MODE_1MUX;
    else if (4 == layout)
        enVoMode = VO_MODE_4MUX;
    /*Indeed only 8 chns show*/
    else if (9 == layout)
     enVoMode = VO_MODE_9MUX;
    else{
        SAMPLE_PRT("preview mode invaild! please try again.\n");

    }
    SAMPLE_PRT(" switch to %d mode\n", enVoMode );
    VO_DEV VoDev =SAMPLE_VO_DEV_DHD0;
    s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
    }

    s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Stop VO failed!\n");
    }
    enPreVoMode = enVoMode;
    VO_PUB_ATTR_S stVoPubAttr;
    memset(&stVoPubAttr, 0, sizeof(stVoPubAttr));

    stVoPubAttr.enIntfSync = VO_OUTPUT_800x600_60;
    stVoPubAttr.enIntfType = VO_INTF_VGA;
    stVoPubAttr.u32BgColor = 0xffff0000;
    stVoPubAttr.bDoubleFrame = HI_TRUE;
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
    }

    s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
    }

}











#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

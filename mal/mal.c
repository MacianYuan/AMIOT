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
#include "sample/common/sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//int Mal_SYS_Init()
//{
//	return 0;
//}

//int Mal_SYS_Exit()
//{
//    SAMPLE_COMM_SYS_Exit();
//	return 0;
//}

//int Mal_VO_GetLayout(int *layout)
//{



//}

//int Mal_VO_SetLayout(int layout)
//{
//    HI_S32 s32Ret;
//    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
//    enVoMode = VO_MODE_1MUX;
//    if (1 == layout)
//        enVoMode = VO_MODE_1MUX;
//    else if (4 == layout)
//        enVoMode = VO_MODE_4MUX;
//    /*Indeed only 8 chns show*/
//    else if (9 == layout)
//     enVoMode = VO_MODE_9MUX;
//    else{
//        SAMPLE_PRT("preview mode invaild! please try again.\n");

//    }
//    SAMPLE_PRT(" switch to %d mode\n", enVoMode );
//    VO_DEV VoDev =SAMPLE_VO_DEV_DHD0;
//    s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);

//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Start VO failed!\n");
//    }
//    enPreVoMode = enVoMode;
//    s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Stop VO failed!\n");
//    }

//    VO_PUB_ATTR_S stVoPubAttr;
//    stVoPubAttr.enIntfSync = VO_OUTPUT_800x600_60;
//    stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
//    stVoPubAttr.u32BgColor = 0x000000ff;
//    stVoPubAttr.bDoubleFrame = HI_TRUE;
//    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Start VO failed!\n");
//    }

//    s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Start VO failed!\n");
//    }

//}

////int Mal_OSD_DrawText(int channel, const char* text, int flags)
////{

////}

////int Mal_Stream_Add_Callback(int channel, Mal_StreamDataCallback cb, void* opaque)
////{

////}

////int Mal_Stream_Del_Callback(int channel, Mal_StreamDataCallback cb, void* opaque)
////{

////}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

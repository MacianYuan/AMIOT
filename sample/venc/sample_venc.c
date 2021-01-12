/******************************************************************************
  A simple program of Hisilicon HI3531 video encode implementation.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-2 Created
******************************************************************************/
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

#include "sample_comm.h"

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_NTSC;
#define SAMPLE_YUV_D1_FILEPATH         "SAMPLE_420_D1.yuv"
#define VPSS_BSTR_CHN     		0
#define VPSS_LSTR_CHN     		1

/******************************************************************************
* function : show usage
******************************************************************************/
void SAMPLE_VENC_Usage(char *sPrgNm)
{
    printf("Usage : %s <index>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0) 4*720p H264/JPEG encode.\n");
    printf("\t 1) 4*cvbs H264 encode.\n");
    printf("\t 2) 4*1080p H264 encode.\n");

    return;
}

/******************************************************************************
* function : to process abnormal case                                         
******************************************************************************/
void SAMPLE_VENC_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}

/******************************************************************************
* function : to process abnormal case - the case of stream venc
******************************************************************************/
void SAMPLE_VENC_StreamHandleSig(HI_S32 signo)
{

    if (SIGINT == signo || SIGTSTP == signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    exit(0);
}

/******************************************************************************
* function :  8*720p H264/JPEG + 8*cif H264 encode
******************************************************************************/
HI_S32 SAMPLE_VENC_4_720p_3520dv300(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_720P;

    HI_S32 s32VpssGrpCnt = 4;
    PAYLOAD_TYPE_E enPayLoad[3]= {PT_H264, PT_JPEG,PT_H264};
    PIC_SIZE_E enSize[3] = {PIC_HD720, PIC_HD720,PIC_CIF};
    HI_U32 u32Profile = 1; /*0: baseline; 1:MP; 2:HP 3:svc-t */
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode;
	VPSS_CHN_MODE_S stVpssChnMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch = '\0';
    SIZE_S stSize;

    /******************************************
     step  1: init variable 
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_HD720, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    stVbConf.u32MaxPoolCnt = 32;
    
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    //stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;
    stVbConf.astCommPool[0].u32BlkCnt = s32VpssGrpCnt * 8;
    memset(stVbConf.astCommPool[0].acMmzName,0,
        sizeof(stVbConf.astCommPool[0].acMmzName));

    
    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_CIF, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    
    memset(stVbConf.astCommPool[1].acMmzName,0,
        sizeof(stVbConf.astCommPool[1].acMmzName));
    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = s32VpssGrpCnt * 6;
    memset(stVbConf.astCommPool[1].acMmzName,0,
        sizeof(stVbConf.astCommPool[1].acMmzName));

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_VENC_8_720p_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VENC_8_720p_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_HD720, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_VENC_8_720p_0;
    }
    
    //stGrpAttr.u32MaxW = stSize.u32Width;
    //stGrpAttr.u32MaxH = stSize.u32Height;
    //stGrpAttr.bDrEn = HI_FALSE;
    //stGrpAttr.bDbEn = HI_FALSE;
    //stGrpAttr.bIeEn = HI_TRUE;
    //stGrpAttr.bNrEn = HI_TRUE;
    //stGrpAttr.bHistEn = HI_TRUE;
    //stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    //stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, s32VpssGrpCnt/4,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_VENC_8_720p_1;
    }
	
	for (i=0; i<s32VpssGrpCnt; i++)
    {
		VpssGrp = i;
		s32Ret = HI_MPI_VPSS_GetChnMode(VpssGrp, VPSS_BSTR_CHN, &stVpssChnMode);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("get Vpss chn mode failed!\n");
	        goto END_VENC_8_720p_2;
	    }
		memset(&stVpssChnMode,0,sizeof(VPSS_CHN_MODE_S));
		stVpssChnMode.enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnMode.u32Width  = stSize.u32Width;
		stVpssChnMode.u32Height = stSize.u32Height;
		stVpssChnMode.stFrameRate.s32DstFrmRate = -1;
		stVpssChnMode.stFrameRate.s32SrcFrmRate = -1;
		stVpssChnMode.enPixelFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
		stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;
		s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VPSS_BSTR_CHN, &stVpssChnMode);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("Set Vpss chn mode failed!\n");
	        goto END_VENC_8_720p_2;
	    }
		
	}


    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_VENC_8_720p_2;
    }

    /******************************************
     step 5: select rc mode
    ******************************************/
    while(1)
    {
        printf("please choose rc mode:\n"); 
        printf("\t0) CBR\n"); 
        printf("\t1) VBR\n"); 
        printf("\t2) FIXQP\n"); 
        ch = getchar();
        if(10 == ch)
        {
            continue;
        }
        getchar();
        if ('0' == ch)
        {
            enRcMode = SAMPLE_RC_CBR;
            break;
        }
        else if ('1' == ch)
        {
            enRcMode = SAMPLE_RC_VBR;
            break;
        }
        else if ('2' == ch)
        {
            enRcMode = SAMPLE_RC_FIXQP;
            break;
        }
        else
        {
            printf("rc mode invaild! please try again.\n");
            continue;
        }
    }
    /******************************************
     step 5: start stream venc (big + little)
    ******************************************/
    for (i=0; i<s32VpssGrpCnt; i++)
    {
        /*** main stream,H264**/
        VencChn = i*2;
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[0],\
                                       gs_enNorm, enSize[0], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_2;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn,VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

       
	   /*** Sub stream **/
        VencChn ++;
        s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[2], \
                                        gs_enNorm, enSize[2], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

		/*** Main stream jpeg**/
        VencChn = 2*(s32VpssGrpCnt + i);
		s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize[1], &stSize);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("Get picture size failed!\n");
	        goto END_VENC_8_720p_3;
	    }
		
        s32Ret = SAMPLE_COMM_VENC_SnapStart(VencChn,&stSize);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

        //s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VPSS_BSTR_CHN);
        //if (HI_SUCCESS != s32Ret)
        //{
        //    SAMPLE_PRT("Start Venc failed!\n");
        //    goto END_VENC_8_720p_3;
        //}
    }

    /******************************************
     step 6: stream venc process -- get stream, then save it to file. 
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(s32VpssGrpCnt*2);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_VENC_8_720p_3;
    }
	
    printf("peress ENTER to capture one picture to file\n");
	while (ch != 'q')
	{
		for (i=0; i<s32VpssGrpCnt; i++)
		{
			/*** main frame **/
			VpssGrp = i;
			VencChn = 2*(s32VpssGrpCnt + i);
			s32Ret = SAMPLE_COMM_VENC_SnapProcess(VencChn, VpssGrp, VPSS_BSTR_CHN);
			if (HI_SUCCESS != s32Ret)
			{
				SAMPLE_PRT("snap process failed!\n");
			}
			printf("snap chn %d ok!\n", i);
		
			//sleep(1);
			
			printf("press 'q' to exit snap process!peress ENTER to capture one picture to file\n");
			if((ch = getchar()) == 'q')
			{
				break;
			}
		}

		
	}
    printf("please press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /******************************************
     step 7: exit process
    ******************************************/
    SAMPLE_COMM_VENC_StopGetStream();
    //for (i=0; i<s32VpssGrpCnt; i++)
    //   {
    //	VencChn = 2*(s32VpssGrpCnt + i);
    //	s32Ret = SAMPLE_COMM_VENC_SnapStop(VencChn);
    //    if (HI_SUCCESS != s32Ret)
    //    {
    //        SAMPLE_PRT("Stop snap failed!\n");
    //        goto END_VENC_8_720p_3;
    //    }
    //}
    
END_VENC_8_720p_3:
	for (i=0; i<s32VpssGrpCnt; i++)
	{
		VencChn = (i+s32VpssGrpCnt)*2;
		VpssGrp = i;
		VpssChn = VPSS_BSTR_CHN;
		SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
		SAMPLE_COMM_VENC_SnapStop(VencChn);
	}

    for (i=0; i<s32VpssGrpCnt*2; i++)
    {
        VencChn = i;
        VpssGrp = i/2;
        VpssChn = (VpssGrp%2)?VPSS_LSTR_CHN:(VPSS_BSTR_CHN);
        SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
        SAMPLE_COMM_VENC_Stop(VencChn);
    }
	
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_VENC_8_720p_2:	
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_VENC_8_720p_1:	
    SAMPLE_COMM_VI_Stop(enViMode);
END_VENC_8_720p_0:	
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}

/******************************************************************************
* function :  8*720p H264/JPEG + 8*cif H264 encode
******************************************************************************/
HI_S32 SAMPLE_VENC_4_1080p_3520dv300(HI_VOID)
{
     SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_1080P;
    VIDEO_NORM_E enNorm = VIDEO_ENCODING_MODE_AUTO;

    PAYLOAD_TYPE_E enPayLoad[3]= {PT_H264, PT_JPEG,PT_H264};
    PIC_SIZE_E enSize[3] = {PIC_HD1080, PIC_HD1080,PIC_CIF};
    HI_U32 u32Profile = 1; /*0: baseline; 1:MP; 2:HP 3:svc-t */
    VB_CONF_S stVbConf;
    VPSS_CHN VpssChn;
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode;
	VPSS_CHN_MODE_S stVpssChnMode;

    HI_U32 u32ViChnCnt = 4;
    HI_S32 s32VpssGrpCnt = 4;
    
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN VpssChn_VoHD0 = VPSS_CHN2;
    VO_DEV VoDev;
	VO_LAYER VoLayer;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr;
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    SAMPLE_VO_MODE_E enVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;
	
	VO_WBC VoWbc;
    VO_WBC_ATTR_S stWbcAttr;    
    VO_WBC_SOURCE_S stWbcSource;   

    /******************************************
     step  1: init variable 
    ******************************************/    
    memset(&stVbConf,0,sizeof(VB_CONF_S));
    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(enNorm,\
                PIC_HD1080, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    stVbConf.u32MaxPoolCnt = 16;//128;

    /* video buffer*/
    //todo: vb=15
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;//8;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_VENC_8_720p_0;
    }

    /******************************************
     step 3: start vi dev & chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VENC_8_720p_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, PIC_HD1080, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_VENC_8_720p_0;
    }

	memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
	stGrpAttr.u32MaxW = stSize.u32Width;
	stGrpAttr.u32MaxH = stSize.u32Height;
	stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
	stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;
    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_VENC_8_720p_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_VENC_8_720p_2;
    }

    /******************************************
     step 5: select rc mode
    ******************************************/
    while(1)
    {
        printf("please choose rc mode:\n"); 
        printf("\t0) CBR\n"); 
        printf("\t1) VBR\n"); 
        printf("\t2) FIXQP\n"); 
        ch = getchar();
        if(10 == ch)
        {
            continue;
        }
        getchar();
        if ('0' == ch)
        {
            enRcMode = SAMPLE_RC_CBR;
            break;
        }
        else if ('1' == ch)
        {
            enRcMode = SAMPLE_RC_VBR;
            break;
        }
        else if ('2' == ch)
        {
            enRcMode = SAMPLE_RC_FIXQP;
            break;
        }
        else
        {
            printf("rc mode invaild! please try again.\n");
            continue;
        }
    }
    /******************************************
     step 5: start stream venc (big + little)
    ******************************************/
    for (i=0; i<s32VpssGrpCnt; i++)
    {
        /*** main stream,H264**/
        VencChn = i*2;
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[0],\
                                       gs_enNorm, enSize[0], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_2;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn,VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

       
	   /*** Sub stream **/
        VencChn ++;
        s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[2], \
                                        gs_enNorm, enSize[2], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VPSS_LSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

		/*** Main stream jpeg**/
/*        VencChn = 2*(s32VpssGrpCnt + i);
		s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize[1], &stSize);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("Get picture size failed!\n");
	        goto END_VENC_8_720p_3;
	    }
		
        s32Ret = SAMPLE_COMM_VENC_SnapStart(VencChn,&stSize);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }
*/
    }

    /******************************************
     step 6: stream venc process -- get stream, then save it to file. 
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(s32VpssGrpCnt*2);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_VENC_8_720p_3;
    }
	

	while (1)
	{


		
	}
    printf("please press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /******************************************
     step 7: exit process
    ******************************************/
    SAMPLE_COMM_VENC_StopGetStream();
    //for (i=0; i<s32VpssGrpCnt; i++)
    //   {
    //	VencChn = 2*(s32VpssGrpCnt + i);
    //	s32Ret = SAMPLE_COMM_VENC_SnapStop(VencChn);
    //    if (HI_SUCCESS != s32Ret)
    //    {
    //        SAMPLE_PRT("Stop snap failed!\n");
    //        goto END_VENC_8_720p_3;
    //    }
    //}
    
END_VENC_8_720p_3:
	for (i=0; i<s32VpssGrpCnt; i++)
	{
		VencChn = (i+s32VpssGrpCnt)*2;
		VpssGrp = i;
		VpssChn = VPSS_BSTR_CHN;
		SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
		SAMPLE_COMM_VENC_SnapStop(VencChn);
	}

    for (i=0; i<s32VpssGrpCnt*2; i++)
    {
        VencChn = i;
        VpssGrp = i/2;
        VpssChn = (VpssGrp%2)?VPSS_LSTR_CHN:(VPSS_BSTR_CHN);
        SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
        SAMPLE_COMM_VENC_Stop(VencChn);
    }
	
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_VENC_8_720p_2:	
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_VENC_8_720p_1:	
    SAMPLE_COMM_VI_Stop(enViMode);
END_VENC_8_720p_0:	
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}
/******************************************************************************
* function :  8*1080p H264 encode
******************************************************************************/
HI_S32 SAMPLE_VENC_4_cvbs_3520dv300(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_16_D1;

    HI_S32 s32VpssGrpCnt = 4;//16;//8
    PAYLOAD_TYPE_E enPayLoad[3]= {PT_H264, PT_JPEG,PT_H264};
    PIC_SIZE_E enSize[3] = {PIC_960H, PIC_HD720,PIC_CIF};
    HI_U32 u32Profile = 1; /*0: baseline; 1:MP; 2:HP 3:svc-t */
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode;
	VPSS_CHN_MODE_S stVpssChnMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch = '\0';
    SIZE_S stSize;

    /******************************************
     step  1: init variable 
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_960H, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    stVbConf.u32MaxPoolCnt = 64;
    
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    //stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;
    stVbConf.astCommPool[0].u32BlkCnt = s32VpssGrpCnt * 12;
    memset(stVbConf.astCommPool[0].acMmzName,0,
        sizeof(stVbConf.astCommPool[0].acMmzName));

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_VENC_8_720p_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VENC_8_720p_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_960H, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_VENC_8_720p_0;
    }
    
    //stGrpAttr.u32MaxW = stSize.u32Width;
    //stGrpAttr.u32MaxH = stSize.u32Height;
    //stGrpAttr.bDrEn = HI_FALSE;
    //stGrpAttr.bDbEn = HI_FALSE;
    //stGrpAttr.bIeEn = HI_TRUE;
    //stGrpAttr.bNrEn = HI_TRUE;
    //stGrpAttr.bHistEn = HI_TRUE;
    //stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    //stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, s32VpssGrpCnt/4,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_VENC_8_720p_1;
    }
	
	for (i=0; i<s32VpssGrpCnt; i++)
    {
		VpssGrp = i;
		s32Ret = HI_MPI_VPSS_GetChnMode(VpssGrp, VPSS_BSTR_CHN, &stVpssChnMode);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("get Vpss chn mode failed!\n");
	        goto END_VENC_8_720p_2;
	    }
		memset(&stVpssChnMode,0,sizeof(VPSS_CHN_MODE_S));
		stVpssChnMode.enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnMode.u32Width  = stSize.u32Width;
		stVpssChnMode.u32Height = stSize.u32Height;
		stVpssChnMode.stFrameRate.s32DstFrmRate = -1;
		stVpssChnMode.stFrameRate.s32SrcFrmRate = -1;
		stVpssChnMode.enPixelFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
		stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;
		s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VPSS_BSTR_CHN, &stVpssChnMode);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("Set Vpss chn mode failed!\n");
	        goto END_VENC_8_720p_2;
	    }
		
	}


    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_VENC_8_720p_2;
    }

    /******************************************
     step 5: select rc mode
    ******************************************/
    while(1)
    {
        printf("please choose rc mode:\n"); 
        printf("\t0) CBR\n"); 
        printf("\t1) VBR\n"); 
        printf("\t2) FIXQP\n"); 
        ch = getchar();
        if(10 == ch)
        {
            continue;
        }
        getchar();
        if ('0' == ch)
        {
            enRcMode = SAMPLE_RC_CBR;
            break;
        }
        else if ('1' == ch)
        {
            enRcMode = SAMPLE_RC_VBR;
            break;
        }
        else if ('2' == ch)
        {
            enRcMode = SAMPLE_RC_FIXQP;
            break;
        }
        else
        {
            printf("rc mode invaild! please try again.\n");
            continue;
        }
    }
    /******************************************
     step 5: start stream venc (big + little)
    ******************************************/
    for (i=0; i<s32VpssGrpCnt; i++)
    {
        /*** main stream,H264**/
        VencChn = i;//i*2
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[0],\
                                       gs_enNorm, enSize[0], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_2;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn,VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_VENC_8_720p_3;
        }


    }

    /******************************************
     step 6: stream venc process -- get stream, then save it to file. 
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(s32VpssGrpCnt);//s32VpssGrpCnt*2
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_VENC_8_720p_3;
    }
	

    printf("please press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /******************************************
     step 7: exit process
    ******************************************/
    SAMPLE_COMM_VENC_StopGetStream();
    //for (i=0; i<s32VpssGrpCnt; i++)
    //   {
    //	VencChn = 2*(s32VpssGrpCnt + i);
    //	s32Ret = SAMPLE_COMM_VENC_SnapStop(VencChn);
    //    if (HI_SUCCESS != s32Ret)
    //    {
    //        SAMPLE_PRT("Stop snap failed!\n");
    //        goto END_VENC_8_720p_3;
    //    }
    //}
    
END_VENC_8_720p_3:
	for (i=0; i<s32VpssGrpCnt; i++)
	{
		VencChn = (i+s32VpssGrpCnt)*2;
		VpssGrp = i;
		VpssChn = VPSS_BSTR_CHN;
		SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
		SAMPLE_COMM_VENC_SnapStop(VencChn);
	}

    for (i=0; i<s32VpssGrpCnt*2; i++)
    {
        VencChn = i;
        VpssGrp = i/2;
        VpssChn = (VpssGrp%2)?VPSS_LSTR_CHN:(VPSS_BSTR_CHN);
        SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
        SAMPLE_COMM_VENC_Stop(VencChn);
    }
	
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_VENC_8_720p_2:	
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_VENC_8_720p_1:	
    SAMPLE_COMM_VI_Stop(enViMode);
END_VENC_8_720p_0:	
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}

/******************************************************************************
* function    : main()
* Description : video venc sample
******************************************************************************/
int main(int argc, char *argv[])
{
    HI_S32 s32Ret;
    if ( (argc < 2) || (1 != strlen(argv[1])))
    {
        SAMPLE_VENC_Usage(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_VENC_HandleSig);
    signal(SIGTERM, SAMPLE_VENC_HandleSig);
    
    switch (*argv[1])
    {
        case '0':/* 8*720p H264/JPEG + 8*cif H264 encode */
            s32Ret = SAMPLE_VENC_4_720p_3520dv300();
            break;
		case '1':/* 16*cvbs H264 encode */
            s32Ret = SAMPLE_VENC_4_cvbs_3520dv300();
            break;
        case '2':/* 4*1080p H264 encode */
			s32Ret = SAMPLE_VENC_4_1080p_3520dv300();

            break;

			
        default:
            printf("the index is invaild!\n");
            SAMPLE_VENC_Usage(argv[0]);
            return HI_FAILURE;
    }
    
    if (HI_SUCCESS == s32Ret)
        printf("program exit normally!\n");
    else
        printf("program exit abnormally!\n");
    exit(s32Ret);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

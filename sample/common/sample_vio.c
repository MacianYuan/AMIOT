/******************************************************************************
  A simple program of Hisilicon HI3520D video input and output implementation.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2012-12 Created
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "sample_vio.h"

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
SAMPLE_VIDEO_LOSS_S gs_stVideoLoss;
HI_U32 gs_u32ViFrmRate = 0;

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void SAMPLE_VIO_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}

/******************************************************************************
* function : video loss detect process
* NOTE: If your ADC stop output signal when NoVideo, you can open VDET_USE_VI macro.
******************************************************************************/

void *SAMPLE_VI_AD26828_VLossDetProc(void *parg)
{
    int fd;
    HI_S32 s32Ret, i, s32ChnPerDev;
    VI_DEV ViDev;
    VI_CHN ViChn;
    tw2865_video_loss video_loss;
    SAMPLE_VI_PARAM_S stViParam;
    SAMPLE_VIDEO_LOSS_S *ctl = (SAMPLE_VIDEO_LOSS_S*)parg;

    fd = open(NVP1918_FILE, O_RDWR);//TW2865_FILE
    if (fd < 0)
    {
        printf("open %s fail\n", NVP1918_FILE);
        ctl->bStart = HI_FALSE;
        return NULL;
    }

    s32Ret = SAMPLE_COMM_VI_Mode2Param(ctl->enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return NULL;
    }
    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;

    while (ctl->bStart)
    {
        for (i = 0; i < stViParam.s32ViChnCnt; i++)
        {
            ViChn = i * stViParam.s32ViChnInterval;
            ViDev = SAMPLE_COMM_VI_GetDev(ctl->enViMode, ViChn);
            if (ViDev < 0)
            {
                SAMPLE_PRT("get vi dev failed !\n");
                return NULL;
            }

            //video_loss.chip = stViParam.s32ViDevCnt;
            video_loss.chip = 0;
            video_loss.ch   = ViChn % s32ChnPerDev;
            //printf("video_loss.chip %d, video_loss.ch %d\n",video_loss.chip,video_loss.ch );
            ioctl(fd, NVP1114A_GET_VIDEO_LOSS, &video_loss);

            if (video_loss.is_lost)
            {
                printf("pic loss\n");
                HI_MPI_VI_EnableUserPic(ViChn);
            }
            else
            {
                HI_MPI_VI_DisableUserPic(ViChn);
            }
        }
        usleep(500000);
    }

    close(fd);
    ctl->bStart = HI_FALSE;

    return NULL;
}


HI_S32 SAMPLE_VI_StartVLossDet(SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 s32Ret;

    gs_stVideoLoss.bStart= HI_TRUE;
    gs_stVideoLoss.enViMode = enViMode;
    s32Ret = pthread_create(&gs_stVideoLoss.Pid, 0, SAMPLE_VI_AD26828_VLossDetProc, &gs_stVideoLoss);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("pthread_create failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



HI_VOID SAMPLE_VI_StopVLossDet()
{
    if (gs_stVideoLoss.bStart)
    {
        gs_stVideoLoss.bStart = HI_FALSE;
        pthread_join(gs_stVideoLoss.Pid, 0);
    }
    return;
}

HI_S32 SAMPLE_VI_SetUserPic(HI_CHAR *pszYuvFile, HI_U32 u32Width, HI_U32 u32Height,
        HI_U32 u32Stride, VIDEO_FRAME_INFO_S *pstFrame)
{
    FILE *pfd;
    VI_USERPIC_ATTR_S stUserPicAttr;

    /* open YUV file */
    pfd = fopen(pszYuvFile, "rb");
    if (!pfd)
    {
        printf("open file -> %s fail \n", pszYuvFile);
        return -1;
    }

    /* read YUV file. WARNING: we only support planar 420) */
    if (SAMPLE_COMM_VI_GetVFrameFromYUV(pfd, u32Width, u32Height, u32Stride, pstFrame))
    {
        return -1;
    }
    fclose(pfd);

    stUserPicAttr.bPub= HI_TRUE;
    stUserPicAttr.enUsrPicMode = VI_USERPIC_MODE_PIC;
    memcpy(&stUserPicAttr.unUsrPic.stUsrPicFrm, pstFrame, sizeof(VIDEO_FRAME_INFO_S));
    if (HI_MPI_VI_SetUserPic(0, &stUserPicAttr))
    {
        return -1;
    }

    printf("set vi user pic ok, yuvfile:%s\n", pszYuvFile);
    return HI_SUCCESS;
}

/******************************************************************************
* function :  VI:8*D1; VO:HD0(HDMI,VGA)+SD0(CVBS)+SD1 video preview
******************************************************************************/
HI_S32 SAMPLE_VIO_8_D1(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_D1;

    HI_U32 u32ViChnCnt = 8;
    HI_S32 s32VpssGrpCnt = 8;

    VB_CONF_S stVbConf;
    VI_CHN ViChn;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN VpssChn_VoHD0 = VPSS_PRE0_CHN;
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr,stVoPubAttrSD;
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;

    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    VO_WBC_ATTR_S stWbcAttr;

    /******************************************
     step  1: init variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;

    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /* video buffer*/
    //todo: vb=15
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;

    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_8D1_0;
    }

    /******************************************
     step 3: start vi dev & chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_8D1_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8D1_0;
    }

    stGrpAttr.u32MaxW = 800;
    stGrpAttr.u32MaxH = 600;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_FALSE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_8D1_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_8D1_2;
    }
    /******************************************
    step 5: start vo HD0(VGA)
    ******************************************/

    printf("start vo HD0.\n");
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_1MUX;

    stVoPubAttr.enIntfSync = VO_OUTPUT_800x600_60;
    stVoPubAttr.enIntfType = VO_INTF_VGA;
    stVoPubAttr.u32BgColor = 0xffff0000;
    stVoPubAttr.bDoubleFrame = HI_TRUE;

    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_8D1_4;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_8D1_5;
    }

    /* if it's displayed on HDMI, we should start HDMI */
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
        {
            SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
            goto END_8D1_5;
        }
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;

        s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_5;
        }
    }

    /******************************************
    step 7: start vo SD0 (CVBS) (WBC target)
    ******************************************/

    /******************************************
    step 8: HD0 switch mode
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DHD0;
    enVoMode = VO_MODE_1MUX;

    printf("HD0 switch mode\n");

    while(1)
    {
        enPreVoMode = enVoMode;
        u32WndNum = 1;
        enVoMode = VO_MODE_1MUX;

        //u32WndNum = 4;
        //enVoMode = VO_MODE_4MUX;

        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }

        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }

        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }
         return s32Ret;
    }

    /******************************************
     step 8: exit process
    ******************************************/

END_8D1_7:
    SAMPLE_COMM_VO_UnBindVoWbc(SAMPLE_VO_DEV_DSD0, 0);
    HI_MPI_VO_DisableWbc(SAMPLE_VO_DEV_DHD0);

END_8D1_6:
    VoDev = SAMPLE_VO_DEV_DSD0;
    VoChn = 0;
    enVoMode = VO_MODE_1MUX;
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_8D1_5:
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_16MUX;
    /*先disableChn ,再解除绑定*/
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_8D1_4:
#if HICHIP == HI3521_V100
    VoDev = SAMPLE_VO_DEV_DSD1;
    VoChn = 0;
    enVoMode = VO_MODE_1MUX;
    SAMPLE_COMM_VO_UnBindVi(VoDev,VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
#endif
END_8D1_3:	//vi unbind vpss
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8D1_2:	//vpss stop
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_8D1_1:	//vi stop
    SAMPLE_COMM_VI_Stop(enViMode);
END_8D1_0:	//system exit
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

/******************************************************************************
* function    : main()
* Description : video preview sample
******************************************************************************/
int sample_vio_main(void)
{
    HI_S32 s32Ret;

    signal(SIGINT, SAMPLE_VIO_HandleSig);
    signal(SIGTERM, SAMPLE_VIO_HandleSig);

    /* VI:8*D1; VO:HD0(HDMI,VGA)+SD0(CVBS)+SD1 video preview. */
    s32Ret = SAMPLE_VIO_8_D1();

    if (HI_SUCCESS == s32Ret)
        printf("program exit normally!\n");
    else
        printf("program exit abnormally!\n");
    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


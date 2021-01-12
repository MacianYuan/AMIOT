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

#include "hisivio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_16_D1;
VIDEO_NORM_E enNorm = VIDEO_ENCODING_MODE_PAL;

HI_U32 u32ViChnCnt = 16;
HI_S32 s32VpssGrpCnt = 16;

VB_CONF_S stVbConf;
VPSS_GRP VpssGrp;
VPSS_GRP_ATTR_S stGrpAttr;
VPSS_CHN VpssChn_VoHD0 = VPSS_CHN2;
VPSS_CHN VpssChn_VoSD0 = VPSS_CHN3;

VO_DEV VoDev;
VO_LAYER VoLayer;
VO_CHN VoChn;
VO_PUB_ATTR_S stVoPubAttr;
VO_VIDEO_LAYER_ATTR_S stLayerAttr;
SAMPLE_VO_MODE_E enVoMode, enPreVoMode;

HI_U32 i;
HI_S32 s32Ret = HI_SUCCESS;
HI_U32 u32BlkSize;
SIZE_S stSize;
HI_U32 u32WndNum;

static struct fb_bitfield g_r32 = {16,8, 0};
static struct fb_bitfield g_g32 = {8, 8, 0};
static struct fb_bitfield g_b32 = {0, 8, 0};
static struct fb_bitfield g_a32 = {24, 8, 0};



void C_Hisi_VIO_FB0_Enable(HI_VOID)
{
    HI_BOOL show;
    HIFB_ALPHA_S stAlpha;
    int fd;
    struct fb_var_screeninfo stVarInfo;

    fd = open("/dev/fb0", O_RDWR, 0);
    show = 0x0;

    ioctl(fd, FBIOPUT_SHOW_HIFB, &show);

    stAlpha.bAlphaEnable = 0x1;
    stAlpha.bAlphaChannel = 0x0;
    stAlpha.u8Alpha0 = 0x0;
    stAlpha.u8Alpha1 = 0xff;
    stAlpha.u8GlobalAlpha = 0xff;
    ioctl(fd, FBIOPUT_ALPHA_HIFB, &stAlpha);
    ioctl(fd, FBIOGET_VSCREENINFO, &stVarInfo);

    stVarInfo.xres = 800;
    stVarInfo.yres = 600;
    stVarInfo.xres_virtual = 800;
    stVarInfo.yres_virtual = 600;
    stVarInfo.transp= g_a32;
    stVarInfo.red = g_r32;
    stVarInfo.green = g_g32;
    stVarInfo.blue = g_b32;
    stVarInfo.bits_per_pixel = 32;

    stVarInfo.activate = FB_ACTIVATE_NOW;
    ioctl(fd, FBIOPUT_VSCREENINFO, &stVarInfo);
    show = 0x1;

    ioctl(fd, FBIOPUT_SHOW_HIFB, &show);
    while(1)
    {
        sleep(100);
    }
}

/******************************************************************************
* function : VI(960H@30: 8 windows) -> VPSS -> HD0(1080P@60)
                                    -> SD0(D1)
******************************************************************************/
HI_S32 C_Hisi_VIO_PAL_Init(int ch_mode)
{
    /******************************************
     step  1: init variable
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    stVbConf.u32MaxPoolCnt = 64;

    /* video buffer*/
    //todo: vb=15
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 4;

    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_8_960H_0;
    }

    /******************************************
     step 3: start vi dev & chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, enNorm);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("start vi failed!\n");
        goto END_8_960H_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, PIC_960H, &stSize);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8_960H_1;
    }

    memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;
    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM, &stGrpAttr);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_8_960H_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_8_960H_2;
    }

    /******************************************
     step 5: start vo HD0 (HDMI+VGA), multi-screen, you can switch mode
    ******************************************/
    printf("start vo HD0.\n");
    VoDev = SAMPLE_VO_DEV_DHD0;
    VoLayer = SAMPLE_VO_LAYER_VHD0;
    u32WndNum = 4;
    enVoMode = ch_mode;

    stVoPubAttr.enIntfSync = VO_OUTPUT_800x600_60;
    stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
    stVoPubAttr.u32BgColor = 0x000000ff;
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_8_960H_3;
    }

    memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync, \
        &stLayerAttr.stImageSize.u32Width, \
        &stLayerAttr.stImageSize.u32Height, \
        &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_8_960H_3;
    }

    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X 	  = 0;
    stLayerAttr.stDispRect.s32Y 	  = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;
    stLayerAttr.bDoubleFrame = HI_TRUE;
    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
        goto END_8_960H_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoLayer, enVoMode);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_8_960H_4;
    }

    for(i=0;i<u32WndNum;i++){
        VoChn = i;
        VpssGrp = i;

        s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_960H_4;
        }
    }

    C_Hisi_VIO_FB0_Enable();

    return s32Ret;

    /******************************************
     step 8: exit process
    ******************************************/
END_8_960H_4:
    VoDev = SAMPLE_VO_DEV_DHD0;
    VoLayer = SAMPLE_VO_LAYER_VHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_16MUX;
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
    }
    SAMPLE_COMM_VO_StopLayer(VoLayer);
    SAMPLE_COMM_VO_StopDev(VoDev);
END_8_960H_3:	//vi unbind vpss
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8_960H_2:	//vpss stop
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_8_960H_1:	//vi stop
    SAMPLE_COMM_VI_Stop(enViMode);
END_8_960H_0: //system exit
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

/******************************************
* function : HD0 switch mode
******************************************/
HI_S32 C_Hisi_Select_Chmode(int ch_PreMode,int ch_Mode)
{
    VoDev = SAMPLE_VO_DEV_DHD0;
    VoLayer = SAMPLE_VO_LAYER_VHD0;
    enVoMode = ch_Mode;

    enPreVoMode = ch_PreMode;

    if (0 == enVoMode)
    {
        u32WndNum = 1;
        enVoMode = VO_MODE_1MUX;
    }
    else if (1 == enVoMode)
    {
        u32WndNum = 4;
        enVoMode = VO_MODE_4MUX;
    }
    SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

    s32Ret= HI_MPI_VO_SetAttrBegin(VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        goto END_8_960H_5;
    }

    s32Ret = SAMPLE_COMM_VO_StopChn(VoLayer, enPreVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        goto END_8_960H_5;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoLayer, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        goto END_8_960H_5;
    }
    s32Ret= HI_MPI_VO_SetAttrEnd(VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        goto END_8_960H_5;
    }

    return s32Ret;
    /******************************************
     step 8: exit process
    ******************************************/
END_8_960H_5:
    VoDev = SAMPLE_VO_DEV_DSD0;
    VoLayer = SAMPLE_VO_LAYER_VSD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_16MUX;
    SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoLayer,VoChn,VpssGrp,VpssChn_VoSD0);
    }
    SAMPLE_COMM_VO_StopLayer(VoLayer);
    SAMPLE_COMM_VO_StopDev(VoDev);

    //vi unbind vpss
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
    //vpss stop
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
    //vi stop
    SAMPLE_COMM_VI_Stop(enViMode);
    //system exit
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}






















#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


/**
* \file gui.c
* \author WooShang <WooShang@126.com>
* \date 2015/07/09
*
* Copyright (C) 2015 Wooshang.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mpi_vo.h>
#include <mpi_vdec.h>
#include <hifb.h>
#include "gui.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_WIDTH_SYNC 1280
#define HI_HEIGHT_SYNC 720

static int fdGUI = -1;
static struct fb_bitfield g_r32 = {16,8, 0};
static struct fb_bitfield g_g32 = {8, 8, 0};
static struct fb_bitfield g_b32 = {0, 8, 0};
static struct fb_bitfield g_a32 = {24, 8, 0};

static int enable_vo(int voDev)
{
   HI_S32 s32Ret;
   VO_PUB_ATTR_S stPubAttr;

   memset(&stPubAttr, 0, sizeof(stPubAttr));
   stPubAttr.u32BgColor = 0xffff0000;
   stPubAttr.enIntfType = VO_INTF_VGA;

   stPubAttr.enIntfSync = VO_OUTPUT_800x600_60;
   stPubAttr.bDoubleFrame = HI_TRUE;

   s32Ret = HI_MPI_VO_SetPubAttr(voDev, &stPubAttr);
   if (s32Ret != HI_SUCCESS){
       printf("failed with %#x!\n", s32Ret);
       return -1;
   }

   s32Ret = HI_MPI_VO_Enable(voDev);
   if (s32Ret != HI_SUCCESS) {
      printf("failed with %#x!\n", s32Ret);
       return -1;
   }
   return 0;

}

static int disable_vo(int voDev)
{
   if(HI_MPI_VO_Disable(voDev) == HI_SUCCESS)
       return 0;
   return -1;
}

int gui_init()
{
   if(enable_vo(0) !=0)
       return -1;

   HI_MPI_VO_GfxLayerUnBindDev(GRAPHICS_LAYER_HC0, 0);
   if (HI_SUCCESS != HI_MPI_VO_GfxLayerBindDev(GRAPHICS_LAYER_HC0, 0)) {
        printf("%s: Graphic Bind to VODev failed!,line:%d\n", __FUNCTION__,  __LINE__);
       return -1;
   }

   fdGUI = open("/dev/fb0", O_RDWR, 0);
   if(fdGUI <0) {
       perror("open");
      return -1;
   }

   for (;;) {
       HI_BOOL bShow = HI_FALSE;
       if (ioctl(fdGUI, FBIOPUT_SHOW_HIFB, &bShow) < 0) {
           printf("FBIOPUT_SHOW_HIFB failed!\n");
           break;
       }

       HIFB_ALPHA_S stAlpha;
       memset(&stAlpha, 9, sizeof(stAlpha));
       stAlpha.bAlphaEnable = HI_TRUE;
       stAlpha.bAlphaChannel = HI_TRUE;
       stAlpha.u8Alpha0 = 0xff;
       stAlpha.u8Alpha1 = 0xff;
       stAlpha.u8GlobalAlpha = 0xff;
       if (ioctl(fdGUI, FBIOPUT_ALPHA_HIFB,  &stAlpha) < 0) {
           printf("Set alpha failed!\n");
           break;
       }
       struct fb_var_screeninfo var;
       if (ioctl(fdGUI, FBIOGET_VSCREENINFO, &var) < 0){
           printf("Get variable screen info failed!\n");
           break;
        }
       var.xres_virtual = HI_WIDTH_SYNC;
       var.yres_virtual = HI_HEIGHT_SYNC;
       var.xres = HI_WIDTH_SYNC;
       var.yres = HI_HEIGHT_SYNC;
       var.transp= g_a32;
       var.red = g_r32;
       var.green = g_g32;
       var.blue = g_b32;
       var.bits_per_pixel = 32;
       var.activate = FB_ACTIVATE_NOW;

       if (ioctl(fdGUI, FBIOPUT_VSCREENINFO, &var) < 0){
           printf("Put variable screen info failed!\n");
           break;
       }

       bShow = HI_TRUE;
       if(ioctl(fdGUI, FBIOPUT_SHOW_HIFB, &bShow) !=0) {
           printf("FBIOPUT_SHOW_HIFB failed\n");
           break;
       }
       return 0;
   }
   close(fdGUI);
   disable_vo(0);
   return -1;
}

int gui_exit()
{
   if(fdGUI!=-1) {
       close(fdGUI);
       fdGUI = -1;
   }
   disable_vo(0);
   return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :main.cpp
* @Platform     :Hi3520D - linux-3.0.8 - Qt4.8.6 - arm-hisiv100nptl-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************29-June-2020(MacianYuan):
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.  2010-2020  All rights reserved.
******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>

#include "hifb.h"
#include "mainwindow.h"
#include "mainservice.h"
#include "sqlworker/sqlworker.h"

static struct fb_bitfield g_r16 = {10, 5, 0};
static struct fb_bitfield g_g16 = {5, 5, 0};
static struct fb_bitfield g_b16 = {0, 5, 0};
static struct fb_bitfield g_a16 = {15, 1, 0};

int macian_fb_init()
{
    HI_S32 s32Ret;
    HI_BOOL bShow;
    //        int i;
    //        unsigned char *pHideScreen;
    HIFB_ALPHA_S stAlpha;
    //        FILE *fp;
    HIFB_COLORKEY_S stColorKey;
    /*
    *调用 open 函数打开指定的 HiFB 设备
    */
    int fd = -1;
    fd = open("/dev/fb0", O_RDWR,0);
    if(fd < 0)
    {
        printf("open /dev/fb0 failed!\n");
       return HI_FAILURE;
    }
    /////////////////////////////////////////
    //设置图层启用压缩功能。
    HI_BOOL g_bCompress = HI_FALSE;
    if (ioctl(fd, FBIOPUT_COMPRESSION_HIFB, &g_bCompress) < 0)
    {
        printf("Func:%s line:%d FBIOPUT_COMPRESSION_HIFB failed!\n", __FUNCTION__, __LINE__);
        close(fd);
        return HI_NULL;
    }
    //////////////////////////////////////
    /*显示或隐藏该叠加层
    *bShow = HI_TRUE：显示当前叠加层
    *bShow = HI_FALSE：隐藏当前叠加层
    */
    bShow = HI_FALSE;
    if (ioctl(fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
    {
        printf("FBIOPUT_SHOW_HIFB failed!\n");
        return HI_NULL;
    }
    printf("open /dev/fb0 success!\n");


    /*2.
    *设置叠加层在屏幕上显示的起始点坐标
    */

    HIFB_POINT_S stPoint = {0, 0};
    if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
    {
        printf("set screen original show position failed!\n");
        close(fd);
        return HI_FAILURE;
    }
    printf("open FBIOPUT_SCREEN_ORIGIN_HIFB success!\n");
    //设置叠加层的 Alpha
    stAlpha.bAlphaEnable = HI_FALSE; //Alpha 叠加使能，默认为 1。
    stAlpha.bAlphaChannel = HI_FALSE;//Alpha 通道使能，默认为 0。
    stAlpha.u8Alpha0 = 0x0;//Alpha0 值，范围 0～255，默认为 255。在 RGB1:5:5:5 格式下，当最高位为 0 时，选择该值作为 Alpha 叠加的 Alpha值。
    stAlpha.u8Alpha1 = 0xff;//Alpha1 值，范围 0～255，默认为 255。在 RGB1:5:5:5 格式下，当最高位为 0 时，选择该值作为 Alpha 叠加的 Alpha值。
    stAlpha.u8GlobalAlpha = 0x80;//全局 Alpha 值，范围为 0～255，默认为 255。在 Alpha 通道使能时起作用
    if (ioctl(fd, FBIOPUT_ALPHA_HIFB,  &stAlpha) < 0)
    {
        printf("Set alpha failed!\n");
                close(fd);
        return HI_NULL;
    }
    printf("open FBIOPUT_ALPHA_HIFB success!\n");


    stColorKey.bKeyEnable = HI_TRUE;
    stColorKey.u32Key = 0x0;
    if (ioctl(fd, FBIOPUT_COLORKEY_HIFB, &stColorKey) < 0)
    {
        printf("FBIOPUT_COLORKEY_HIFB!\n");
        close(fd);
        return HI_NULL;
    }
    printf("open FBIOPUT_COLORKEY_HIFB success!\n");



    /*3. 获取屏幕的可变信息。*/
    struct fb_var_screeninfo var;

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
    {
        printf("Get variable screen info failed!\n");
        close(fd);
        return HI_FAILURE;
    }

    /*4. 设置 Framebuffer 的屏幕分辨率和像素格式等
     * the screen size: IMAGE_WIDTH*IMAGE_HEIGHT
     * the virtual screen size: IMAGE_WIDTH*(IMAGE_HEIGHT*2)
     * the pixel format: ARGB1555
     */

    var.xres_virtual = 800;
    var.yres_virtual = 600;
    var.xres = 800;
    var.yres = 600;
    var.xoffset = 0;
    var.yoffset = 0;
    var.transp= g_a16;
    var.red = g_r16;
    var.green = g_g16;
    var.blue = g_b16;
    var.bits_per_pixel = 16;
    var.activate = FB_ACTIVATE_NOW;
    s32Ret = ioctl(fd, FBIOPUT_VSCREENINFO, &var);
    //s32Ret = ioctl (fd,FBIOPAN_DISPLAY, &var);
    if (s32Ret < 0)
    {
        printf("Put variable screen info failed!\n");
        close(fd);
        return HI_FAILURE;
    }
    printf("GUI init");

    /*获取 Framebuffer 的固定信息。
    *id 设备驱动名称。
    *smem_start 显存起始物理地址。
    *smem_len 显存大小。
    *type 显卡类型。 固定为FB_TYPE_PACKED_PIXELS，表示像素值紧密排列。
    *type_aux 附加类型。 不支持，在FB_TYPE_PACKED_PIXELS 显卡类型下无含义。
    *visual 色彩模式。 不支持，默认为FB_VISUAL_TRUECOLOR，真彩色。
    *xpanstep 支持水平方向上的 PAN 显示：   0：不支持。 非 0：支持，此时该值用于表示在水平方向上每步进的像素值。固定为 1
    *ypanstep 支持垂直方向上的 PAN 显示：0：不支持。非 0：支持，此时该值用于表示在垂直方向上每步进的像素值。固定为 1。
    *ywrapstep 该方式类似于 ypanstep，不同之处在于：当其显示到底部时，能回到显存的开始处进行显示。不支持，默认为 0。
    *line_length 每行字节数。 支持。
    *mmio_start 显存映射 I/O 首地址。 不支持，默认为 0。
    *mmio_len 显存映射 I/O 长度。 不支持，默认为 0。
    *accel 显示所支持的硬件加速设备。 不支持，默认为FB_ACCEL_NONE，无加速设备。
    *reserved 保留。 不支持，缺省值为 0。
    */
    struct fb_fix_screeninfo fix;
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix) < 0)
    {
        printf("Get fix screen info failed!\n");
        close(fd);
        return HI_NULL;
    }
    printf("1111");

    /*调用 mmap 函数将物理显存映射到虚拟内存空间。*/
    void *pShowScreen;
    pShowScreen = mmap(HI_NULL, fix.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(MAP_FAILED == pShowScreen)
    {
        printf("mmap framebuffer failed!\n");
        close(fd);
        return HI_NULL;
    }

    /*操作虚拟内存，完成具体的绘制任务。*/
    memset(pShowScreen, 0x00, fix.smem_len);
    bShow = HI_TRUE;

    /*调用 munmap 解除显存映射。*/
    if (ioctl(fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
    {
        printf("FBIOPUT_SHOW_HIFB failed!\n");
        munmap(pShowScreen, fix.smem_len);
        return HI_NULL;
    }
    return 0;
}

void signal_handle(int signo)
{

    if (SIGINT == signo || SIGTSTP == signo) {
        Mal_SYS_Exit();
        qDebug()<<"program exit abnormally!";
    }
    exit(-1);
}


int main(int argc, char *argv[])
{
    //初始化Linux FB
    macian_fb_init();
    //字体设置
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    //主窗口显示
    MainWindow *mainwindow = new MainWindow();
    mainwindow->showFullScreen();


    //主服务运行
    MainService *mainservice = new MainService;
    mainservice->init();

    //系统逻辑与显示更新
    QObject::connect(mainservice,SIGNAL(menushowSelect()),mainwindow,SLOT(on_menushowSelect()));
    QObject::connect(mainservice,SIGNAL(stackedshowSelect(bool)),mainwindow,SLOT(on_stackedshowSelect(bool)));
    QObject::connect(mainservice,SIGNAL(stream_change(bool)),mainwindow,SLOT(on_stream_change(bool)));
    QObject::connect(mainservice,SIGNAL(gps_signal_update()),mainwindow,SLOT(on_gps_signal_update()));
    QObject::connect(mainservice,SIGNAL(label_time_refresh(QString)),mainwindow,SLOT(on_label_time_refresh(QString)));



    int result =  a.exec();
    Mal_SYS_Exit();

    return result;
}


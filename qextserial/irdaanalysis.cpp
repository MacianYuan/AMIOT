#include "irdaanalysis.h"

#define MENU_KEY    0xe619ff00
#define UP_KEY      0xea15ff00
#define DOWN_KEY    0xe718ff00
#define LEFT_KEY    0xe916ff00
#define RIGHT_KEY   0xf20dff00
#define VOL1_KEY    0xa55aff00
#define VOL2_KEY    0xf708ff00
#define SET_KEY     0xbb44ff00
#define EXIT_KEY    0xbc43ff00
#define POEWR_KEY   0xb847ff00
#define MUTE_KEY    0xba45ff00


static hiir_dev_param static_dev_param[] =
{
    /*NEC with simple repeat code : uPD6121G*/
    {828, 972, 414, 486, 45, 67 , 135, 203, 180, 270, 32, 0, HIIR_DEFAULT_FREQ},
    //{728, 1050, 350, 550, 25, 75, 100, 250, 150, 300, 32, 0, HIIR_DEFAULT_FREQ},

    /*NEC with simple repeat code : D6121/BU5777/D1913*/
    {828, 972, 414, 486, 45, 67 , 135, 203, 207, 243, 32, 0, HIIR_DEFAULT_FREQ},

    /*NEC with simple repeat code : LC7461M-C13*/
    {828, 972, 414, 486, 45, 67 , 135, 203, 207, 243, 42, 0, HIIR_DEFAULT_FREQ},

    /*NEC with simple repeat code : AEHA*/
    {270, 405, 135, 203, 34, 51 , 101, 152, 270, 405, 48, 0, HIIR_DEFAULT_FREQ},

    /*TC9012 : TC9012F/9243*/
    {414, 486, 414, 486, 45, 67 , 135, 203, 0  , 0  , 32, 1, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : uPD6121G*/
    {828, 972, 414, 486, 45, 67 , 135, 203, 0  , 0  , 32, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : LC7461M-C13*/
    {828, 972, 414, 486, 45, 67 , 135, 203, 0  , 0  , 42, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : MN6024-C5D6*/
    {270, 405, 270, 405, 68, 101, 203, 304, 0  , 0  , 22, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : MN6014-C6D6*/
    {279, 419, 279, 419, 70, 105, 140, 210, 0  , 0  , 24, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : MATNEW*/
    {279, 419, 300, 449, 35, 52 , 105, 157, 0  , 0  , 48, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : MN6030*/
    {279, 419, 279, 419, 70, 105, 210, 314, 0  , 0  , 22, 2, HIIR_DEFAULT_FREQ},

    /*NEC with full repeat code : PANASONIC*/
    {282, 422, 282, 422, 70, 106, 211, 317, 0  , 0  , 22, 2, HIIR_DEFAULT_FREQ},

    /*SONY-D7C5*/
    {192, 288, 48 , 72 , 48, 72 , 96 , 144, 0  , 0  , 12, 3, HIIR_DEFAULT_FREQ},

    /*SONY-D7C6*/
    {192, 288, 48 , 72 , 48, 72 , 96 , 144, 0  , 0  , 13, 3, HIIR_DEFAULT_FREQ},

    /*SONY-D7C8*/
    {192, 288, 48 , 72 , 48, 72 , 96 , 144, 0  , 0  , 15, 3, HIIR_DEFAULT_FREQ},

    /*SONY-D7C13*/
    {192, 288, 48 , 72 , 48, 72 , 96 , 144, 0  , 0  , 20,  3, HIIR_DEFAULT_FREQ},
};

IrdaAnalysis::IrdaAnalysis(QObject *parent):
    QObject(parent)
{
    //红外键盘解码初始化
    m_fd = irda_init();
    printf(" open /dev/Hi_IR success");
    if(m_fd < 0){
        printf(" open /dev/Hi_IR failed");
    }

    //红外键盘编码配置 NEC with full repeat code
    ir_config_fun(m_fd, static_dev_param[5]);

    //用来监听系统文件操作，将操作转换为Qt事件进入系统的消息循环队列。并调用预先设置的事件接受函数，处理事件
    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect (m_notifier, SIGNAL(activated(int)), this, SLOT(on_irda_read()));
}


void IrdaAnalysis::run()
{
//    //红外键盘解码初始化
//    m_fd = irda_init();
//    printf(" open /dev/Hi_IR success");
//    if(m_fd < 0){
//        printf(" open /dev/Hi_IR failed");
//    }

//    //红外键盘编码配置 NEC with full repeat code
//    ir_config_fun(m_fd, static_dev_param[5]);

//    //用来监听系统文件操作，将操作转换为Qt事件进入系统的消息循环队列。并调用预先设置的事件接受函数，处理事件
//    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
//    connect (m_notifier, SIGNAL(activated(int)), this, SLOT(on_irda_read()));
}


//按键编码解析 通过信号槽 链接到其他类中实现控制
void IrdaAnalysis::normal_report_irkey(irkey_info_s rcv_irkey_info)
{
    //printf("RECEIVE ---> irkey_datah=0x%.8x, irkey_datal=0x%.8x\t", (int)(rcv_irkey_info.irkey_datah), (int)(rcv_irkey_info.irkey_datal));
    if(rcv_irkey_info.irkey_datal == MENU_KEY){
         printf("KEYUP...MENU_KEY");
         emit menushowSelect();

    }else if(rcv_irkey_info.irkey_datal == UP_KEY){
         printf("KEYUP...UP_KEY");
         emit stackedshowSelect(true);

    }else if(rcv_irkey_info.irkey_datal == DOWN_KEY){
         printf("KEYUP...DOWN_KEY");
         emit stackedshowSelect(false);

    }else if(rcv_irkey_info.irkey_datal == LEFT_KEY){
         emit stream_change(true);
         printf("KEYUP...LEFT_KEY");
    }else if(rcv_irkey_info.irkey_datal == RIGHT_KEY){
         emit stream_change(false);
         printf("KEYUP...RIGHT_KEY");
    }

    if(rcv_irkey_info.irkey_state_code == 1){
        printf("KEYUP...");
    }
    printf("\n");
}
//红外键盘解码初始化
int IrdaAnalysis::irda_init()
{
    const char *device = "/dev/Hi_IR";
    int fd = 0;
    fd = ::open(device, O_RDWR|O_NONBLOCK);
    if(fd < 0)
        return -1;
    return fd;
}

//红外键盘编码配置 NEC with full repeat code
void IrdaAnalysis::on_irda_read()
{
      unsigned int res = 0;
      irkey_info_s rcv_irkey_info;
      //printf("random press  keys...\n");

      res = read(m_fd, &rcv_irkey_info, sizeof(rcv_irkey_info));
      if( (res > 0) && (res <= sizeof(rcv_irkey_info)) ){
          normal_report_irkey(rcv_irkey_info);
      }
      else{
          printf("Hi_IR_FUNC_TEST_007 Error. read irkey device error. res=%d.\n", res);
      }
}

//红外键盘编码配置 NEC with full repeat code
inline void IrdaAnalysis::ir_config_fun(int filp, hiir_dev_param dev_param)
{
    int tmp[2];

    ioctl(filp, IR_IOC_SET_CODELEN, dev_param.code_len);

    ioctl(filp, IR_IOC_SET_FORMAT, dev_param.codetype);

    ioctl(filp, IR_IOC_SET_FREQ, dev_param.frequence);

    tmp[0] = dev_param.leads_min;
    tmp[1] = dev_param.leads_max;
    ioctl(filp, IR_IOC_SET_LEADS, tmp);

    tmp[0] = dev_param.leade_min;
    tmp[1] = dev_param.leade_max;
    ioctl(filp, IR_IOC_SET_LEADE, tmp);

    tmp[0] = dev_param.sleade_min;
    tmp[1] = dev_param.sleade_max;
    ioctl(filp, IR_IOC_SET_SLEADE, tmp);

    tmp[0] = dev_param.cnt0_b_min;
    tmp[1] = dev_param.cnt0_b_max;
    ioctl(filp, IR_IOC_SET_CNT0_B, tmp);

    tmp[0] = dev_param.cnt1_b_min;
    tmp[1] = dev_param.cnt1_b_max;
    ioctl(filp, IR_IOC_SET_CNT1_B, tmp);
}

IrdaAnalysis::~IrdaAnalysis()
{
    //printf("Hi_IR_FUNC_TEST_007 pass.\n\n");
    close(m_fd);
}

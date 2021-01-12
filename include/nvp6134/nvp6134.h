/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: Common header file
*  Description	: This file is common header file
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/


#ifdef HI_GPIO_I2C
#include "gpio_i2c.h"
#endif

#ifdef HI_I2C
//#include "hi_i2c.h"
#undef I2C_INTERNAL
#endif


#ifdef HI_GPIO_I2C
//#define  I2CReadByte   gpio_i2c_read
//#define  I2CWriteByte  gpio_i2c_write

#else
unsigned char __I2CReadByte8(unsigned char devaddress, unsigned char address);
void __I2CWriteByte8(unsigned char devaddress, unsigned char address, unsigned char data);
unsigned char gpio_i2c_read(unsigned char devaddress, unsigned char address);
void          gpio_i2c_write(unsigned char devaddress, unsigned char address, unsigned char value);

#define  gpio_i2c_read   gpio_i2c_read
#define  gpio_i2c_write  gpio_i2c_write

#endif

// device address define
#define NVP6134_R0_ID 	0x91
#define NVP6134B_R0_ID 	0x90   //6134B AND 6134C USES THE SAME CHIPID,DIFF IN REV_ID
#define NVP6134B_REV_ID 	0x00
#define NVP6134C_REV_ID 	0x01	//76
#define NVP6134E_REV_ID 	0x03	//128
#define NVP6134F_REV_ID 	0x04	//88
#define CH_PER_CHIP		4

#define NTSC		0x00
#define PAL			0x01

#define AHD_PELCO_16BIT

#define PELCO_CMD_RESET			0
#define PELCO_CMD_SET			1
#define PELCO_CMD_UP			2
#define PELCO_CMD_DOWN			3
#define PELCO_CMD_LEFT			4
#define PELCO_CMD_RIGHT			5
#define PELCO_CMD_OSD			6
#define PELCO_CMD_IRIS_OPEN		7
#define PELCO_CMD_IRIS_CLOSE	8
#define PELCO_CMD_FOCUS_NEAR	9
#define PELCO_CMD_FOCUS_FAR		10
#define PELCO_CMD_ZOOM_WIDE		11
#define PELCO_CMD_ZOOM_TELE		12
#define PELCO_CMD_SCAN_SR		13
#define PELCO_CMD_SCAN_ST		14
#define PELCO_CMD_PRESET1		15
#define PELCO_CMD_PRESET2		16
#define PELCO_CMD_PRESET3		17
#define PELCO_CMD_PTN1_SR		18
#define PELCO_CMD_PTN1_ST		19
#define PELCO_CMD_PTN2_SR		20
#define PELCO_CMD_PTN2_ST		21
#define PELCO_CMD_PTN3_SR       22
#define PELCO_CMD_PTN3_ST       23
#define PELCO_CMD_RUN           24

//other command
#define EXC_CMD_RESET			0
#define EXC_CMD_SET				1
#define EXC_CMD_UP				2
#define EXC_CMD_DOWN			3
#define EXC_CMD_LEFT			4
#define EXC_CMD_RIGHT			5
#define EXC_CMD_IRIS_OPEN		7
#define EXC_CMD_IRIS_CLOSE		8
#define EXC_CMD_FOCUS_NEAR		9
#define EXC_CMD_FOCUS_FAR		10
#define EXC_CMD_ZOOM_WIDE		11
#define EXC_CMD_ZOOM_TELE		12
#define EXC_CMD_SCAN_SR			13
#define EXC_CMD_SCAN_ST			14
#define EXC_CMD_PRESET1			15
#define EXC_CMD_PRESET2			16
#define EXC_CMD_PRESET3			17
#define EXC_CMD_PTN1_SR			18
#define EXC_CMD_PTN1_ST			19
#define EXC_CMD_PTN2_SR			20
#define EXC_CMD_PTN2_ST			21
#define EXC_CMD_PTN3_SR       	22
#define EXC_CMD_PTN3_ST       	23
#define EXC_CMD_RUN        	   	24
#define EXC_CMD_OSD_ENTER		6
#define EXC_CMD_ZOOM_STOP		25
#define EXC_CMD_OSD_OPEN		26
#define EXC_CMD_OSD_CLOSE		27

#define EXT_CMD_RESET			0
#define EXT_CMD_SET				1
#define EXT_CMD_UP				2
#define EXT_CMD_DOWN			3
#define EXT_CMD_LEFT			4
#define EXT_CMD_RIGHT			5
#define EXT_CMD_OSD				6
#define EXT_CMD_IRIS_OPEN		7
#define EXT_CMD_IRIS_CLOSE		8
#define EXT_CMD_FOCUS_NEAR		9
#define EXT_CMD_FOCUS_FAR		10
#define EXT_CMD_ZOOM_WIDE		11
#define EXT_CMD_ZOOM_TELE		12
#define EXT_CMD_SCAN_SR			13
#define EXT_CMD_SCAN_ST			14
#define EXT_CMD_PRESET1			15
#define EXT_CMD_PRESET2			16
#define EXT_CMD_PRESET3			17
#define EXT_CMD_PTN1_SR			18
#define EXT_CMD_PTN1_ST			19
#define EXT_CMD_PTN2_SR			20
#define EXT_CMD_PTN2_ST			21
#define EXT_CMD_PTN3_SR       	22
#define EXT_CMD_PTN3_ST       	23
#define EXT_CMD_RUN        	   	24
#define EXT_CMD_VER_SWITCH     	25

#define SET_ALL_CH          0xff

//FIXME HI3520 Register
#define VIU_CH_CTRL					0x08
#define VIU_ANC0_START				0x9c
#define VIU_ANC0_SIZE				0xa0
#define VIU_ANC1_START				0xa4
#define VIU_ANC1_SIZE				0xa8
#define VIU_BLANK_DATA_ADDR			0xac

#define IOC_VDEC_SET_VIDEO_MODE			0x07
#define IOC_VDEC_GET_INPUT_VIDEO_FMT	0x08  
#define IOC_VDEC_GET_VIDEO_LOSS     	0x09
#define IOC_VDEC_SET_SYNC		     	0x0A
#define IOC_VDEC_SET_EQUALIZER			0x0B
#define IOC_VDEC_GET_DRIVERVER			0x0C
#define IOC_VDEC_PTZ_ACP_READ			0x0D
#define IOC_VDEC_SET_BRIGHTNESS	    	0x0E
#define IOC_VDEC_SET_CONTRAST   		0x0F
#define IOC_VDEC_SET_HUE    			0x10
#define IOC_VDEC_SET_SATURATION  		0x11
#define IOC_VDEC_SET_SHARPNESS  		0x12
#define IOC_VDEC_SET_CHNMODE    		0x13
#define IOC_VDEC_SET_OUTPORTMODE  		0x14
#define IOC_VDEC_PTZ_CHANNEL_SEL		0x20
#define IOC_VDEC_PTZ_PELCO_INIT			0x21
#define IOC_VDEC_PTZ_PELCO_RESET		0x22
#define IOC_VDEC_PTZ_PELCO_SET			0x23
#define IOC_VDEC_PTZ_PELCO_UP			0x24
#define IOC_VDEC_PTZ_PELCO_DOWN			0x25
#define IOC_VDEC_PTZ_PELCO_LEFT			0x26
#define IOC_VDEC_PTZ_PELCO_RIGHT		0x27
#define IOC_VDEC_PTZ_PELCO_OSD			0x28
#define IOC_VDEC_PTZ_PELCO_IRIS_OPEN	0x29
#define IOC_VDEC_PTZ_PELCO_IRIS_CLOSE	0x2a
#define IOC_VDEC_PTZ_PELCO_FOCUS_NEAR	0x2b
#define IOC_VDEC_PTZ_PELCO_FOCUS_FAR	0x2c
#define IOC_VDEC_PTZ_PELCO_ZOOM_WIDE	0x2d
#define IOC_VDEC_PTZ_PELCO_ZOOM_TELE	0x2e
#define IOC_VDEC_ACP_WRITE              0x2f
#define IOC_VDEC_ACP_WRITE_EXTENTION    0x30
#define IOC_VDEC_PTZ_ACP_READ_EACH_CH	0x31

#define IOC_VDEC_INIT_MOTION			0x40
#define IOC_VDEC_ENABLE_MOTION			0x41
#define IOC_VDEC_DISABLE_MOTION			0x42
#define IOC_VDEC_SET_MOTION_AREA		0x43
#define IOC_VDEC_GET_MOTION_INFO		0x44
#define IOC_VDEC_SET_MOTION_DISPLAY		0x45
#define IOC_VDEC_SET_MOTION_SENS		0x46

#define IOC_AUDIO_SET_CHNNUM            0x80
#define IOC_AUDIO_SET_SAMPLE_RATE       0x81
#define IOC_AUDIO_SET_BITWIDTH          0x82
#define IOC_VDEC_SET_I2C				0x83

#define IOC_VDEC_ACP_POSSIBLE_FIRMUP	0xA0	// by Andy(2016-06-26)
#define IOC_VDEC_ACP_CHECK_ISPSTATUS    0xA1	// by Andy(2016-07-12)
#define IOC_VDEC_ACP_START_FIRMUP	    0xA2	// by Andy(2016-07-12)
#define IOC_VDEC_ACP_FIRMUP				0xA3	// by Andy(2016-06-26)
#define IOC_VDEC_ACP_FIRMUP_END			0xA4	// by Andy(2016-06-26)

#define IOC_VDEC_GET_ADC_CLK			0xB1
#define IOC_VDEC_SET_ADC_CLK			0xB2


typedef struct _nvp6134_video_mode
{
    unsigned int chip;
    unsigned int mode;
	unsigned char vformat[16];
	unsigned char chmode[16];
}nvp6134_video_mode;

typedef struct _nvp6134_chn_mode
{
    unsigned char ch;
	unsigned char vformat;
	unsigned char chmode;
}nvp6134_chn_mode;

typedef struct _nvp6134_opt_mode
{
	unsigned char chipsel;
    unsigned char portsel;
	unsigned char portmode;
	unsigned char chid;
}nvp6134_opt_mode;

typedef struct _nvp6134_input_videofmt
{
    unsigned int inputvideofmt[16];
	unsigned int getvideofmt[16];
	unsigned int geteqstage[16];
	unsigned int getacpdata[16][8];
}nvp6134_input_videofmt;

typedef struct _nvp6134_input_videofmt_ch
{
	unsigned char ch;
	nvp6134_input_videofmt vfmt;
}nvp6134_input_videofmt_ch;

typedef struct _nvp6124_i2c_mode
{
	unsigned char flag;       // 0: read, 1 : write
	unsigned char slaveaddr;
	unsigned char bank;
	unsigned char address;
	unsigned char data;
}nvp6124_i2c_mode;

typedef struct _nvp6134_video_adjust
{
    unsigned int ch;
	unsigned int value;
}nvp6134_video_adjust;

typedef struct _nvp6134_motion_area
{
    unsigned char ch;
    int m_info[12];
}nvp6134_motion_area;

typedef struct _nvp6134_audio_playback
{
    unsigned int chip;
    unsigned int ch;
}nvp6134_audio_playback;

typedef struct _nvp6134_audio_da_mute
{
    unsigned int chip;
}nvp6134_audio_da_mute;

typedef struct _nvp6134_audio_da_volume
{
    unsigned int chip;
    unsigned int volume;
}nvp6134_audio_da_volume;

typedef struct _nvp6134_audio_format
{
	unsigned char format;   /* 0:i2s; 1:dsp */
    unsigned char mode;   /* 0:slave 1:master*/
	unsigned char dspformat; /*0:dsp;1:ssp*/
    unsigned char clkdir;  /*0:inverted;1:non-inverted*/
	unsigned char chn_num; /*2,4,8,16*/
	unsigned char bitrate; /*0:256fs 1:384fs invalid for nvp6114 2:320fs*/
	unsigned char precision;/*0:16bit;1:8bit*/
	unsigned char samplerate;/*0:8kHZ;1:16kHZ; 2:32kHZ*/
} nvp6134_audio_format;

// by Andy(2016-06-26)
typedef struct __file_information
{
	unsigned int	channel;
  	unsigned char 	filename[64];
  	unsigned char 	filePullname[64+32];
  	unsigned int	filesize;
  	unsigned int	filechecksum;			// (sum of file&0x0000FFFFF)
  	unsigned int	currentpacketnum;		// current packet sequnce number(0,1,2........)
  	unsigned int	filepacketnum;			// file packet number = (total size/128bytes), if remain exist, file packet number++
  	unsigned char 	onepacketbuf[128+32];

  	unsigned int	currentFileOffset;		// Current file offset
  	unsigned int	readsize;				// currnet read size

  	unsigned int	ispossiblefirmup[16]; 	// is it possible to update firmware?
  	int 			result;

  	int				appstatus[16];			// Application status

} FIRMWARE_UP_FILE_INFO, *PFIRMWARE_UP_FILE_INFO;

enum __CABLE_TYPE_INFORMATION__
{
	CABLE_TYPE_COAX=0,
	CABLE_TYPE_UTP,

	CABLE_TYPE_MAX
};

enum __DETECTION_TYPE_INFORMATION__
{
	DETECTION_TYPE_AUTO=0,
	DETECTION_TYPE_AHD,
	DETECTION_TYPE_CHD,
	DETECTION_TYPE_THD,
	DETECTION_TYPE_CVBS,

	DETECTION_TYPE_MAX
};

#define NVP6134_IOC_MAGIC            'n'

#define NVP6134_SET_AUDIO_PLAYBACK   		_IOW(NVP6134_IOC_MAGIC, 0x21, nvp6134_audio_playback) 
#define NVP6134_SET_AUDIO_DA_MUTE    		_IOW(NVP6134_IOC_MAGIC, 0x22, nvp6134_audio_da_mute)
#define NVP6134_SET_AUDIO_DA_VOLUME  		_IOW(NVP6134_IOC_MAGIC, 0x23, nvp6134_audio_da_volume)
/*set record format*/
#define NVP6134_SET_AUDIO_R_FORMAT     		_IOW(NVP6134_IOC_MAGIC, 0x24, nvp6134_audio_format)
/*set playback format*/
#define NVP6134_SET_AUDIO_PB_FORMAT     	_IOW(NVP6134_IOC_MAGIC, 0x25, nvp6134_audio_format)

#define SET_BIT(data,bit) ((data)|=(1<<(bit)))
#define CLE_BIT(data,bit) ((data)&=(~(1<<(bit))))



/********************************************************************
 *  define and enum
 ********************************************************************/
typedef enum _nvp6134_vi_mode
{
	NVP6134_VI_720H			= 0x00,	//720x576i(480)
	NVP6134_VI_960H,       			//960x576i(480)
	NVP6134_VI_1280H,				//1280x576i(480)
	NVP6134_VI_1440H,				//1440x576i(480)
	NVP6134_VI_1920H,				//1920x576i(480)
	NVP6134_VI_960H2EX,   			//3840x576i(480)
	NVP6134_VI_720P_2530	= 0x10,	//1280x720@25p(30)
	NVP6134_VI_EXC_720P,			//1280x720@25p(30)
	NVP6134_VI_EXT_720PA,			//1280x720@25p(30)	
	NVP6134_VI_EXT_720PB,			//1280x720@25p(30)
	NVP6134_VI_HDEX,				//2560x720@25p(30)
	NVP6134_VI_EXC_HDEX,			//2560x720@25p(30)
	NVP6134_VI_EXT_HDAEX,			//2560x720@25p(30)
	NVP6134_VI_EXT_HDBEX,			//2560x720@25p(30)
	NVP6134_VI_720P_5060,			//1280x720@50p(60)
	NVP6134_VI_EXC_720PRT,			//1280x720@50p(60)
	NVP6134_VI_EXT_720PRT,			//1280x720@50p(60)
	NVP6134_VI_M_720PRT,			//for nextchip test only
	NVP6134_VI_1080P_2530	= 0x20,	//1920x1080@25p(30)
	NVP6134_VI_EXC_1080P,			//1920x1080@25p(30)
	NVP6134_VI_EXT_1080P,			//1920x1080@25p(30)
	NVP6134_VI_1080P_NRT,			//1920x1080@12.5p(15)
	NVP6134_VI_1080P_NOVIDEO,		//1920x1080@25p(30)
	NVP6134_VI_3M_NRT		= 0x30, //2048x1536@18p
	NVP6134_VI_3M,					//2048x1536@25p(30p)
	NVP6134_VI_EXC_3M_NRT,			//reserved
	NVP6134_VI_EXC_3M,				//reserved
	NVP6134_VI_EXT_3M_NRT,			//2048x1536@18p
	NVP6134_VI_EXT_3M,				//reserved
	NVP6134_VI_4M_NRT		= 0x40, //2560x1440@NRT
	NVP6134_VI_4M,					//2560x1440@25p(30)
	NVP6134_VI_EXC_4M_NRT,			//reserved
	NVP6134_VI_EXC_4M,				//reserved
	NVP6134_VI_EXT_4M_NRT,			//reserved
	NVP6134_VI_EXT_4M,				//reserved
	NVP6134_VI_5M_NRT		= 0x50,	//2592x1944@12.5P
	NVP6134_VI_5M,					//2592x1944@25P	
	NVP6134_VI_EXC_5M_NRT,			//reserved
	NVP6134_VI_EXC_5M,				//reserved
	NVP6134_VI_EXT_5M_NRT,			//2592x1944@12.5P
	NVP6134_VI_EXT_5M,				//reserved
	NVP6134_VI_5M_20P,				//2592x1944@20P
	NVP6134_VI_8M_NRT		= 0x80,	//reserved
	NVP6134_VI_8M,					//reserved
	NVP6134_VI_EXC_8M_NRT,			//reserved
	NVP6134_VI_EXC_8M,				//reserved
	NVP6134_VI_EXT_8M_NRT,			//reserved
	NVP6134_VI_EXT_8M,				//reserved
	NVP6134_VI_UHD_X,				//reserved
	NVP6134_VI_BUTT
}NVP6134_VI_MODE;

typedef enum _nvp6134_outmode_sel
{
	NVP6134_OUTMODE_1MUX_SD = 0,
	NVP6134_OUTMODE_1MUX_HD,
	NVP6134_OUTMODE_1MUX_HD5060,
	NVP6134_OUTMODE_1MUX_FHD,
	NVP6134_OUTMODE_2MUX_SD,
	NVP6134_OUTMODE_2MUX_HD_X,
	NVP6134_OUTMODE_2MUX_HD,
	NVP6134_OUTMODE_2MUX_FHD_X,
	NVP6134_OUTMODE_4MUX_SD,
	NVP6134_OUTMODE_4MUX_HD_X,
	NVP6134_OUTMODE_4MUX_HD,
	NVP6134_OUTMODE_2MUX_FHD,
	NVP6134_OUTMODE_1MUX_HD_X,  
	NVP6134_OUTMODE_1MUX_FHD_X,
	NVP6134_OUTMODE_4MUX_FHD_X,
	NVP6134_OUTMODE_4MUX_MIX,
	NVP6134_OUTMODE_2MUX_MIX,
	NVP6134_OUTMODE_1MUX_BT1120S,
	NVP6134_OUTMODE_1MUX_3M_RT,
	NVP6134_OUTMODE_1MUX_4M_NRT,
	NVP6134_OUTMODE_BUTT
}NVP6134_OUTMODE_SEL;


/********************************************************************
 *  structure
 ********************************************************************/

/********************************************************************
 *  external api
 ********************************************************************/
void nvp6134_common_init(unsigned char chip);
int nvp6134_set_portmode(const unsigned char chip, const unsigned char portsel, const unsigned char portmode, const unsigned char chid);
int nvp6134_set_chnmode(const unsigned char ch, const unsigned char vfmt, const unsigned char chnmode);
void nvp6134_set_portcontrol(unsigned char chip, unsigned char portsel, unsigned char enclk, unsigned char endata);
///////������ͨ��ģʽ����
void nvp6134_set_common_value(unsigned char ch, int mode);
void nvp6134_setchn_common_cvbs(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_common_720p(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_common_fhd(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_720h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_960h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_1280h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_1440h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_1920h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_3840h(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_720p(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_720pex(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_720p5060(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_M_720p5060(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_1080p2530(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exc_720p(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exc_720pex(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exc_720p5060(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exc_1080p2530(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exta_720p(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_exta_720pex(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_extb_720p(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_extb_720pex(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ext_720p5060(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ext_1080p2530(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_3MNRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ext_3MNRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_3M(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_5MNRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ext_5MNRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_1080p_NRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_1080p_novideo(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_QHD(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_QHD_X(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_QHD_NRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_5M_20p(const unsigned char ch, const unsigned char vfmt);

void nvp6134_setchn_ahd_8M_NRT(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ahd_UHDX(const unsigned char ch, const unsigned char vfmt);
void nvp6134_setchn_ext_bsf(const unsigned char ch, const unsigned char vfmt);

unsigned int  video_fmt_det(nvp6134_input_videofmt *pvideofmt);
unsigned char video_fmt_debounce(unsigned char ch, unsigned char keep_fmt, unsigned int keep_sync_width);
unsigned int nvp6134_getvideoloss(void);

void nvp6134_video_set_contrast(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6134_video_set_brightness(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6134_video_set_saturation(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6134_video_set_hue(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6134_video_set_sharpness(unsigned int ch, unsigned int value);
void nvp6134_video_set_ugain(unsigned int ch, unsigned int value);
void nvp6134_video_set_vgain(unsigned int ch, unsigned int value);
void nvp6134_video_set_adcclk(unsigned char ch, unsigned char value);
unsigned char nvp6134_video_get_adcclk(unsigned char ch);
void nvp6134_hide_ch(unsigned char ch);
void nvp6134_show_ch(unsigned char ch);

void nvp6134_VD_chnRst(unsigned char ch);
int nvp6134_GetAgcLockStatus(unsigned char ch);
int nvp6134_GetFSCLockStatus(unsigned char ch);
void nvp6134_ResetFSCLock(unsigned char ch);
void nvp6134_chn_killcolor(unsigned char ch, unsigned char onoff);
void nvp6134_cvbs_slicelevel_con(unsigned char ch, int onoff);
void nvp6134_acp_RT2NRT(unsigned char ch, unsigned char vfmt);
void nvp6134_acp_NRT2RT(unsigned char ch);
void nvp6134_acp_RT2NRT_extension(unsigned char ch);
void nvp6134_acp_NRT2RT_extension(unsigned char ch);
int nvp6134_acp_SetVFmt(unsigned char ch, const unsigned char vfmt);
unsigned char nvp6134_vfmt_convert(unsigned char vdsts, unsigned char g_ck_fmt);
int isItAHDmode( unsigned char vfmt );
unsigned char trans_ahd_to_chd( unsigned char vfmt );

void nvp6134_dump_reg(unsigned char ch, unsigned char bank);

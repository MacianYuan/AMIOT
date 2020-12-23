/*
 * RTC sample&test code.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>

#include "hi_rtc.h"
#include "rtc_test.h"
//void usage(void)
//{
//	printf(
//			"\n"
//			"Usage: ./test [options] [parameter1] ...\n"
//			"Options: \n"
//			"	-s(set)		Set time/alarm,     e.g '-s time 2012/7/15/13/37/59'\n"
//			"	-g(get)		Get time/alarm,     e.g '-g alarm'\n"
//			"	-w(write)	Write RTC register, e.g '-w <reg> <val>'\n"
//			"	-r(ead)		Read RTC register,  e.g '-r <reg>'\n"
//			"	-a(alarm)	Alarm ON/OFF',      e.g '-a ON'\n"
//			"	-reset		RTC reset\n"
//			"	-c(ompensation) temperature compensation ON/OFF, eg '-c ON'\n"
//			"	-f(requency)    frequency precise adjustment, eg '-f <val>'\n"
//			"	-m(mode)	Mode of temperature gather, e.g '-m <mode> <temp>, mode[0-2]'\n"
//			"\n");
//	exit(1);
//}


static int _atoul(const char *str, unsigned char *pvalue)
{
    unsigned int result=0;

    while (*str)
    {
        if (isdigit((int)*str))
        {
            if ((result<429496729) || ((result==429496729) && (*str<'6')))
            {
                result = result*10 + (*str)-48;
            }
            else
            {
                *pvalue = result;
                return -1;
            }
        }
        else
        {
            *pvalue=result;
            return -1;
        }
        str++;
    }
    *pvalue=result;
    return 0;
}


#define ASC2NUM(ch) (ch - '0')
#define HEXASC2NUM(ch) (ch - 'A' + 10)

static int  _atoulx(const char *str, unsigned char *pvalue)
{
    unsigned int   result=0;
    unsigned char  ch;

    while (*str)
    {
        ch=toupper(*str);
        if (isdigit(ch) || ((ch >= 'A') && (ch <= 'F' )))
        {
            if (result < 0x10000000)
            {
                result = (result << 4) + ((ch<='9')?(ASC2NUM(ch)):(HEXASC2NUM(ch)));
            }
            else
            {
                *pvalue=result;
                return -1;
            }
        }
        else
        {
            *pvalue=result;
            return -1;
        }
        str++;
    }

    *pvalue=result;
    return 0;
}

/*used for convert hex value from string to int*/
static int str_to_num(const char *str, unsigned char *pvalue)
{
    if ( *str == '0' && (*(str+1) == 'x' || *(str+1) == 'X') ){
        if (*(str+2) == '\0'){
            return -1;
        }
        else{
            return _atoulx(str+2, pvalue);
        }
    }
    else {
        return _atoul(str,pvalue);
    }
}

/*used for convert time frome string to struct rtc_time_t*/
static int parse_string(char *string, rtc_time_t *p_tm)
{
    char *comma, *head;
    int value[10];
    int i;

    if (!string || !p_tm)
        return -1;

    if (!strchr(string, '/'))
        return -1;

    head = string;
    i = 0;
    comma = NULL;

    for(;;) {
        comma = strchr(head, '/');

        if (!comma){
            value[i++] = atoi(head);
            break;
        }

        *comma = '\0';
        value[i++] = atoi(head);
        head = comma+1;
    }

    if (i < 5)
        return -1;

    p_tm->year   = value[0];
    p_tm->month  = value[1];
    p_tm->date   = value[2];
    p_tm->hour   = value[3];
    p_tm->minute = value[4];
    p_tm->second = value[5];
    p_tm->weekday = 0;

    return 0;
}

int rtc_set(rtc_time_t tm)
{
    int ret = -1;
    int fd = -1;
    const char *dev_name = "/dev/rtc";
    char string[50];


    fd = open(dev_name, O_RDWR);
    if (!fd) {
        printf("open %s failed\n", dev_name);
        return -1;
    }
//    strcpy(string, argv);

//    ret = parse_string(string, &tm);
//    if (ret < 0){
//        printf("parse time param failed\n");
//        goto err1;
//    }
    printf("set time\n");
#if 1
    /* code */
    printf("year:%d\n", tm.year);
    printf("month:%d\n",tm.month);
    printf("date:%d\n", tm.date);
    printf("hour:%d\n", tm.hour);
    printf("minute:%d\n", tm.minute);
    printf("second:%d\n", tm.second);
#endif
    ret = ioctl(fd, HI_RTC_SET_TIME, &tm);
    if (ret < 0) {
        printf("ioctl: HI_RTC_SET_TIME failed\n");
        close(fd);
        return -1;
    }
    printf("ioctl: HI_RTC_SET_TIME succeed\n");

    close(fd);
    return 0;
}


rtc_time_t rtc_get()
{
    rtc_time_t tm;
    int ret = -1;
    int fd = -1;
    const char *dev_name = "/dev/rtc";


    fd = open(dev_name, O_RDWR);
    if (!fd) {
        printf("open %s failed\n", dev_name);
    }
    ret = ioctl(fd, HI_RTC_RD_TIME, &tm);
    if (ret < 0) {
        printf("ioctl: HI_RTC_RD_TIME failed\n");
        //goto err1;
    }

//    printf("Current time value: \n");
//    printf("year %d\n", tm.year);
//    printf("month %d\n", tm.month);
//    printf("date %d\n", tm.date);
//    printf("hour %d\n", tm.hour);
//    printf("minute %d\n", tm.minute);
//    printf("second %d\n", tm.second);
//    printf("weekday %d\n", tm.weekday);

    close(fd);
    return tm;
}






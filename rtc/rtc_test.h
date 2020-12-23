#ifndef RTC_TEST_H
#define RTC_TEST_H
#if defined(__cplusplus)
extern "C" {
#endif

#include "hi_rtc.h"


int rtc_set(rtc_time_t tm);
rtc_time_t rtc_get();


#if defined(__cplusplus)
}
#endif
#endif // RTC_TEST_H

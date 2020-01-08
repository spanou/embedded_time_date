#ifndef __RTC_BASE_H__
#define __RTC_BASE_H__


bool isYearLeap(uint32_t year);
uint32_t tmInSeconds(struct tm time, bool adjustForUTC);

#endif//__RTC_BASE_H__
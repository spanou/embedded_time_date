#ifndef __RTC_BASE_H__
#define __RTC_BASE_H__


bool isYearLeap(unsigned long year);
unsigned long tmInSeconds(struct tm time, bool adjustForUTC);

#endif//__RTC_BASE_H__
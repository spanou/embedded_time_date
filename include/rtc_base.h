#ifndef __RTC_BASE_H__
#define __RTC_BASE_H__


#ifdef __STDC_VERSION__
	#if(__STDC_VERSION__ >= 199901L)
		#define INLINE inline
	#else
		#define INLINE __inline__
	#endif
#else
	#define INLINE
#endif

bool isYearLeap(uint32_t year);
uint32_t tmInSeconds(struct tm time, bool adjustForUTC);

#endif

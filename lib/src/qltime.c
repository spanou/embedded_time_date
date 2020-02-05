/*=============================================================================
 * Copyright (c) 2019, Sakis Panou <sakis.panou@gmail.com>
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *===========================================================================*/
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#include "qltime.h"
#include "qllogger.h"

static const uint8_t monDayCount[] = {
    /*Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec*/
      31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31
};

/*static const uint8_t daysOfWeekValue[] = {
    Sun, Mon, Tue, Wed, Thu, Fri, Sat
      0,   1,   2,   3,   4,   5,    6
};*/

static const uint8_t monOfYearValue[] = {
    /*Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec*/
      1,   4,   4,   0,   2,   5,   0,   3,   6,   1,   4,   6
};

static const uint8_t centuries[] = {
    /*1700, 1800, 1900, 2000*/
      17,   18,   19,   20
};

static const uint8_t centuryCode[] = {
    /*1700, 1800, 1900, 2000*/
      4,    2,    0,    6
};

typedef enum _centuryRangeStatus {
    LOWER_RANGE = -1,
    HIGHER_RANGE = -2,
    IN_RANGE = 0
} CenturyRangeStatus;

typedef enum _centuryRangeIndex {
    RANGE_1700s = 0,
    RANGE_1800s = 1,
    RANGE_1900s = 2,
    RANGE_2000s = 3
} CenturyRangeIndex;

static uint32_t yearsSinceEpoch(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t yearDayFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t hoursFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t minsFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t monthOfYearFromYday(const uint32_t yday,
    uint32_t remainingTimeInSec,
    bool isYearLeap);

static uint8_t yearInCode(uint32_t year);

static INLINE uint8_t monthToValue(uint8_t month){
    return((month > (NMONTH_YEAR-1))?0:monOfYearValue[month]);
}

static INLINE const uint32_t centuryToCode(int32_t idx){
    return(centuryCode[idx]);
}

bool isYearLeap(uint32_t year) {
    bool ret = false;

    if((0 == (year % NLEAP_YEAR))) {
        ret = true;
        if(0 == (year % NYEAR_CENTURY)){
            ret = false;
            if(0 == (year % QTR_CENTENARY)){
                ret = true;
            }
        }
    }

    return(ret);
}

Status tmInSeconds(uint32_t *tmInSecs, struct tm t){

    /* Current Year - 1970 */
    const uint32_t yearsInTotal = (t.tm_year + YEAROFFSET_TM)
        - EPOCH;

    /* Count all the leap years since the epoch */
    uint32_t leapYearCounter = 0;
    uint32_t i;

    if(tmInSecs == NULL){
        return(-INVLDPTR);
    }

    for(i = 0; i < yearsInTotal; i++){
        uint32_t year = EPOCH + i;
        /* Is this a leap year ? */
        if(isYearLeap(year)){
            ++leapYearCounter;
        }
    }

    /*
     * The time in seconds for all the years since
     * 1970 + the additional leap days.
     */
    *tmInSecs = ((NSEC_YEAR * yearsInTotal) +
        (leapYearCounter * NSEC_DAY));
    /* The time in seconds for all the days for the current year */
    *tmInSecs += (t.tm_yday * NSEC_DAY);
    /* The time in seconds for all the hours in the current day */
    *tmInSecs += (t.tm_hour * NSEC_HOUR);
    /* The time in seconds for the minutes since the current hour */
    *tmInSecs += (t.tm_min * NSEC_MIN);
    /* The time in seconds from the current minute */
    *tmInSecs += t.tm_sec;

    /* The value should match: */
    return(NOERROR);
}

Status secondsInStuctTm(struct tm *t, const uint32_t tmInSecs){

    uint32_t remainingTimeInSec=0;

    if(t == NULL){
        return(-INVLDPTR);
    }

    t->tm_year = (yearsSinceEpoch(tmInSecs,
        &remainingTimeInSec) + EPOCH) - YEAROFFSET_TM;

    t->tm_yday = yearDayFromSec(remainingTimeInSec, &remainingTimeInSec);

    t->tm_mon = monthOfYearFromYday(t->tm_yday, remainingTimeInSec,
        isYearLeap(t->tm_year + YEAROFFSET_TM));

#if(0)
    t->tm_mday = 0;
    t->tm_hour = hoursFromSec(remainingTimeInSec, &remainingTimeInSec);
    t->tm_min = minsFromSec(remainingTimeInSec, &remainingTimeInSec);
    t->tm_sec = remainingTimeInSec;

    dayOfWeek((t->tm_year + YEAROFFSET_TM), t->tm_mon, t->tm_mday,
        (uint8_t*)&t->tm_wday);
#else
    t->tm_mday =0;
    t->tm_hour =0;
    t->tm_min =0;
    t->tm_sec =0;
    t->tm_wday= 0;
#endif

    return(NOERROR);
}

Status dayOfWeek(uint32_t year, uint8_t month, uint8_t day, uint8_t* wday){

    uint32_t x = ((year % NYEAR_CENTURY) / NLEAP_YEAR) + day;

    if(wday == NULL){
        return(-INVLDPTR);
    }

    x += monthToValue(month);

    if(true == isYearLeap(year) && (month <= Feb)) {
        x -= 1 ;
    }

    x += yearInCode(year);
    x += (year % NYEAR_CENTURY);
    x = (x % NDAY_WEEK);

    /* Value of 1 means Sunday, value of 0 means Saturday */
    *wday = (x)?(x-1):Sat;

    qlLogger(INFO, "(%d) %s::%s -> dayOfWeek(%d, %d, %d, %d)\n",
        __LINE__, __FILE__, __FUNCTION__, year, month, day, *wday);

    return(NOERROR);
}

static CenturyRangeStatus isCenturyInRange(uint32_t year){
    uint32_t reqCentury = year/NYEAR_CENTURY;

    if(reqCentury > centuries[RANGE_2000s])
        return(HIGHER_RANGE);

    if(reqCentury < centuries[RANGE_1700s])
        return(LOWER_RANGE);

    return(IN_RANGE);
}

static uint8_t yearInCode(uint32_t year){

    int8_t code = isCenturyInRange(year);
    uint32_t yearCache = year;
    uint8_t i = 0;

    /* Move Up or Down by 400 years to get into range*/
    while(code < 0){

        if(code == HIGHER_RANGE){
            yearCache -= QTR_CENTENARY;
        } else if (code == LOWER_RANGE){
            yearCache += QTR_CENTENARY;
        }
        code = isCenturyInRange(yearCache);
    }

    /*Since we are now in range determine the code*/
    for(i=0; i < 4; i++){
        if(centuries[i] == (yearCache/NYEAR_CENTURY)){
            code = i;
            break;
        }
    }

    return(centuryToCode(code));
}

uint32_t yearsSinceEpoch(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec){

    uint32_t yearCount = 0;
    uint32_t leapYearCount = 0;

    *remainingTimeInSec = timeInSeconds;

    while(*remainingTimeInSec >= NSEC_YEAR){

        if(isYearLeap(EPOCH+yearCount)){

            if(*remainingTimeInSec >= NSEC_YEAR + NSEC_DAY) {
                *remainingTimeInSec -= NSEC_YEAR + NSEC_DAY;
                ++leapYearCount;
            } else {
                break;
            }

        } else {
            *remainingTimeInSec -= NSEC_YEAR;
        }

        ++yearCount;
    }

    qlLogger(INFO, "(%d) %s::%s -> Total Year Count: %u\n",
        __LINE__, __FILE__, __FUNCTION__, yearCount);

    qlLogger(INFO, "(%d) %s::%s -> Leap Year Count: %u\n",
        __LINE__, __FILE__, __FUNCTION__, leapYearCount);

    qlLogger(INFO, "(%d) %s::%s -> Non Leap Year Count: %u\n",
        __LINE__, __FILE__, __FUNCTION__, yearCount-leapYearCount);

    return(yearCount);
}

uint32_t yearDayFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    if(timeInSeconds == 0){
        *remainingTimeInSec = 0;
        return(0);
    }

    *remainingTimeInSec= timeInSeconds % NSEC_DAY;

    return((timeInSeconds / NSEC_DAY));
}

uint32_t hoursFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % NSEC_HOUR;

    return(timeInSeconds / NSEC_HOUR);
}

uint32_t minsFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % NSEC_MIN;

    return(timeInSeconds / NSEC_MIN);
}



uint32_t monthOfYearFromYday(uint32_t yDay, uint32_t remainingTimeInSec, bool isYearLeap){

    uint32_t i;
    uint32_t totalDayCount = 0;
    uint8_t monInDays = 0;
    uint32_t monthOfYear = 0;

    /*Check to see if we rolled over to the next day*/
    if(remainingTimeInSec >=0)
        yDay +=1;

    for(i=0; i<NMONTH_YEAR; i++){
        monInDays =
            ((i==Feb) && (true==isYearLeap))?(monDayCount[i] +1):monDayCount[i];

        if((totalDayCount + monInDays) >= yDay){
            monthOfYear = i;
            break;
        } else {
            totalDayCount += monInDays;
        }
    }

    return(monthOfYear);
}

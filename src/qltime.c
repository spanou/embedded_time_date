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
 * ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *===========================================================================*/
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include "qltime.h"

static const uint8_t monDayCount[] = {
    /*Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec*/
      31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31
};

static const uint8_t daysOfWeekValue[] = {
    /*Sun, Mon, Tue, Wed, Thu, Fri, Sat*/
      0,   1,   2,   3,   4,   5,    6
};

static const uint8_t monOfYearValue[] = {
    /*Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec*/
      1,   4,   4,   0,   2,   5,   0,   3,   6,   1,   4,   6
};

static const uint32_t centuries[] = {
    /*1700, 1800, 1900, 2000*/
      17,   18,   19,   20
};

static const uint32_t centuryCode[] = {
    /*1700, 1800, 1900, 2000*/
      4,    2,    0,    6
};

typedef enum _centuryRangeStatus {
    RANGE_LOW = -1,
    RANGE_HIGH = -2,
    RANGE_1700s = 0,
    RANGE_1800s = 1,
    RANGE_1900s = 2,
    RANGE_2000s = 3
} RangeStatus;

static uint32_t yearsSinceEpoch(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t daysFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t hoursFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t minsFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec);

static uint32_t monthFromDayOfYear(const uint32_t dayOfYear,
    uint32_t* daysRemaining,
    bool isYearLeap);

static uint32_t yearCode(uint32_t year);

static INLINE uint8_t monthToValue(uint8_t month){
    return((month > 11)?0:monOfYearValue[month]);
}

static INLINE const uint32_t centuryToCode(int32_t idx){
    return(centuryCode[idx]);
}

bool isYearLeap(uint32_t year) {
    bool ret = false;

    if( (0 == (year % 4)) ) {
        ret = true;
        if(0 == (year % 100)){
            ret = false;
            if(0 == (year % 400)){
                ret = true;
            }
        }
    }

    return(ret);
}

Status tmInSeconds(uint32_t *tmInSecs, struct tm t){

    /* Current Year - 1970 */
    const uint32_t yearsInTotal = (t.tm_year + yearInStructTm)
        - epochYear;

    /* Count all the leap years since the epoch */
    uint32_t leapYearCount = 0;
    uint32_t i;

    if(tmInSecs == NULL){
        return(-INVLDPTR);
    }

    for(i = 0; i < yearsInTotal; i++){
        uint32_t year = epochYear + i;
        /* Is this a leap year ? */
        if(isYearLeap(year)){
            ++leapYearCount;
        }
    }

    /*
     * The time in seconds for all the years since
     * 1970 + the additional leap days.
     */
    *tmInSecs = ((secondsInYear * yearsInTotal) +
        (leapYearCount * secondsInDay));
    /* The time in seconds for all the days for the current year */
    *tmInSecs += (t.tm_yday * secondsInDay);
    /* The time in seconds for all the hours in the current day */
    *tmInSecs += (t.tm_hour * secondsInHour);
    /* The time in seconds for the minutes since the current hour */
    *tmInSecs += (t.tm_min * secondsInMin);
    /* The time in seconds from the current minute */
    *tmInSecs += t.tm_sec;

    /* The value should match: */
    return(NOERROR);
}

Status secondsInStuctTm(struct tm *t, const uint32_t tmInSecs){

    uint32_t remainingTimeInSec=0;
    uint32_t daysRemaining = 0;

    if(t == NULL){
        return(-INVLDPTR);
    }

    t->tm_year = (yearsSinceEpoch(tmInSecs,
        &remainingTimeInSec) + 1970) - 1900;

    t->tm_yday = daysFromSec(remainingTimeInSec, &remainingTimeInSec);

    t->tm_mon = monthFromDayOfYear(t->tm_yday, &daysRemaining,
        isYearLeap(t->tm_year + 1900));

    t->tm_mday = daysRemaining;

    t->tm_hour = hoursFromSec(remainingTimeInSec, &remainingTimeInSec);
    t->tm_min = minsFromSec(remainingTimeInSec, &remainingTimeInSec);
    t->tm_sec = remainingTimeInSec;

    dayOfWeek((t->tm_year + 1900), t->tm_mon, t->tm_mday,
        (uint8_t*)&t->tm_wday);

    return(NOERROR);
}

Status dayOfWeek(uint32_t year, uint8_t month, uint8_t day, uint8_t* wday){

    uint32_t x = ((year % 100) / 4) + day;

    if(wday == NULL){
        return(-INVLDPTR);
    }

    x += monthToValue(month);

    if(true == isYearLeap(year) && (month <= 1)) {
        x -= 1 ;
    }

    x += yearCode(year);
    x += (year %100);
    x = (x%7);

    /* Value of 1 means Sunday, value of 0 means Saturday */
    *wday = (x)?(x-1):Sat;

    return(NOERROR);
}

static RangeStatus isCenturyInRange(uint32_t year){
    uint32_t reqCentury = year/100;
    uint8_t i = 0;


    if(reqCentury > centuries[RANGE_2000s])
        return(RANGE_HIGH);

    if(reqCentury < centuries[RANGE_1700s])
        return(RANGE_LOW);

    for(i=0; i < 4; i++){
        if(centuries[i] == reqCentury){
            return(i);
        }
    }

    return(0); /*Will never reach this point*/
}

static uint32_t yearCode(uint32_t year){

    int32_t code = isCenturyInRange(year);
    uint32_t yearCache = year;

    while(code < 0){

        if(code == RANGE_HIGH){
            yearCache -= 400;
        } else if (code == RANGE_LOW){
            yearCache += 400;
        }

        code = isCenturyInRange(yearCache);
    }
    /*  TODO: Need to fix the types and possiblity of getting a negative value */
    return(centuryToCode(code));
}

uint32_t yearsSinceEpoch(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec){

    uint32_t yearCount = 0;
    *remainingTimeInSec = timeInSeconds;

    while(*remainingTimeInSec >= secondsInLeapYear){

        if(isYearLeap(epochYear+yearCount)){
            *remainingTimeInSec -= secondsInYear + secondsInDay;
        } else {
            *remainingTimeInSec -= secondsInYear;
        }

        ++yearCount;
    }

    return(yearCount);
}

uint32_t daysFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInDay;
    /* Days don't start from 0 */
    return((timeInSeconds / secondsInDay)+1);
}

uint32_t hoursFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInHour;

    return(timeInSeconds / secondsInHour);
}

uint32_t minsFromSec(const uint32_t timeInSeconds,
    uint32_t *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInMin;

    return(timeInSeconds / secondsInMin);
}

uint32_t monthFromDayOfYear(uint32_t dayOfYear, uint32_t* daysRemaining,
    bool isYearLeap){

    uint32_t i;
    uint32_t totalDayCount = 0;
    uint8_t monInDays = 0;
    uint32_t monthOfYear = 0;

    for(i=0; i<12; i++){

        monInDays =
            ((i==Feb) && (true==isYearLeap))?(monDayCount[i] +1):monDayCount[i];

        if((totalDayCount + monInDays) >= dayOfYear){
            *daysRemaining = dayOfYear - totalDayCount;
            monthOfYear = i;
            break;
        } else {
            totalDayCount += monInDays;
        }
    }

    return(monthOfYear);
}

/*  https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week */
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#include "rtc_base.h"

struct tm secondsInTm(struct tm *timeStruct, uint32_t seconds);

bool leapYearChecker(const uint32_t* goldenLeapYearTable,
    const uint32_t goldenLeapYearTableSz,
    uint32_t startYear,
    uint32_t endYear);

void printTable(const uint32_t* table,
    const uint32_t tableSize,
    const uint8_t rowLength);

bool dayOfTheWeekChecker(void);


struct tm secondsInStuctTm(const uint32_t timeInSeconds);

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

static void printStructTm(const struct tm theTime);

const static uint32_t epochYear = 1970;
const static uint32_t secondsInDay = 86400;
const static uint32_t secondsInYear = 31536000;
const static uint32_t secondsInLeapYear = 31622400;
const static uint32_t secondsInHour = 3600;
const static uint32_t secondsInMin = 60;
const static uint32_t yearInStructTm = 1900;
const static uint32_t startYearRange = 1600;
const static uint32_t endYearRange = 2400;
const static uint32_t tableRowSize = 10;

const static uint32_t max32BitVal = 4294967295;
const static uint32_t half32BitVal = 2147483647;

/*
 * Leap Years from 1800-2400
 * https://kalender-365.de/leap-years.php
 * https://miniwebtool.com/leap-years-list/?start_year=1800&end_year=2400
*/
static uint32_t leapYearGoldenTestTable[] = {
    1600, 1604, 1608, 1612, 1616, 1620, 1624, 1628, 1632, 1636,
    1640, 1644, 1648, 1652, 1656, 1660, 1664, 1668, 1672, 1676,
    1680, 1684, 1688, 1692, 1696, 1704, 1708, 1712, 1716, 1720,
    1724, 1728, 1732, 1736, 1740, 1744, 1748, 1752, 1756, 1760,
    1764, 1768, 1772, 1776, 1780, 1784, 1788, 1792, 1796, 1804,
    1808, 1812, 1816, 1820, 1824, 1828, 1832, 1836, 1840, 1844,
    1848, 1852, 1856, 1860, 1864, 1868, 1872, 1876, 1880, 1884,
    1888, 1892, 1896, 1904, 1908, 1912, 1916, 1920, 1924, 1928,
    1932, 1936, 1940, 1944, 1948, 1952, 1956, 1960, 1964, 1968,
    1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004, 2008,
    2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044, 2048,
    2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088,
    2092, 2096, 2104, 2108, 2112, 2116, 2120, 2124, 2128, 2132,
    2136, 2140, 2144, 2148, 2152, 2156, 2160, 2164, 2168, 2172,
    2176, 2180, 2184, 2188, 2192, 2196, 2204, 2208, 2212, 2216,
    2220, 2224, 2228, 2232, 2236, 2240, 2244, 2248, 2252, 2256,
    2260, 2264, 2268, 2272, 2276, 2280, 2284, 2288, 2292, 2296,
    2304, 2308, 2312, 2316, 2320, 2324, 2328, 2332, 2336, 2340,
    2344, 2348, 2352, 2356, 2360, 2364, 2368, 2372, 2376, 2380,
    2384, 2388, 2392, 2396, 2400
};

enum monthsOfYear{
    Jan = 0,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};

enum daysOfWeek {
    Sun=0,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat
};

static const char* monStrings[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};


static const char* monAbrvStrings[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

static const uint8_t monDayCount[] = {
    31,
    28, /* Leap Year 29 */
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

static const char* dayOfWeekStrings[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
};

static const uint8_t daysOfWeekValue[] = {
    0, /*Sunday*/
    1, /*Monday*/
    2, /*Tuesday*/
    3, /*Wednesday*/
    4, /*Thursday*/
    5, /*Friday*/
    6, /*Saturday*/
};

static const uint8_t monOfYearValue[] = {
   1, /* Jan */
   4, /* Feb */
   4, /* Mar */
   0, /* Apr */
   2, /* May */
   5, /* Jun */
   0, /* Jul */
   3, /* Aug */
   6, /* Sep */
   1, /* Oct */
   4, /* Nov */
   6, /* Dec */
};

static const uint32_t centuries[] = {
    17, /*1700*/
    18, /*1800*/
    19, /*1900*/
    20  /*2000*/
};

static const uint32_t centuryCode[] = {
    4, /*1700s*/
    2, /*1800s*/
    0, /*1900s*/
    6, /*2000s*/
};

static inline uint8_t monthToValue(uint8_t month){
    return((month > 11)?0:monOfYearValue[month]);
}

static inline const char* const monthToString(uint8_t month){
    return((month > 11)? NULL : monStrings[month]);
}

static inline const char* const monthToStringAbrv(uint8_t month){
    return((month > 11)? NULL : monAbrvStrings[month]);
}

static inline const char* const dayOfWeekToString(uint8_t dayOfWeek){
    return((dayOfWeek > 6)? NULL : dayOfWeekStrings[dayOfWeek]);
}

static inline const uint32_t centuryToCode(int32_t idx){
    return(centuryCode[idx]);
}

static const int32_t LOWER_RANGE = -1;
static const int32_t HIGHER_RANGE = -2;

static int32_t isCenturyInRange(uint32_t year){
    uint32_t reqCentury = year/100;
    uint8_t i = 0;


    if(reqCentury > centuries[3])
        return(HIGHER_RANGE);

    if(reqCentury < centuries[0])
        return(LOWER_RANGE);

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

        if(code == HIGHER_RANGE){
            yearCache -= 400;
        } else if (code == LOWER_RANGE){
            yearCache += 400;
        }

        code = isCenturyInRange(yearCache);
    }
    /*  TODO: Need to fix the types and possiblity of getting a negative value */
    return(centuryToCode(code));
}

/*
 * http://mathforum.org/dr.math/faq/faq.calendar.html
 * Calculates the day of the week utilizing the  Key Value Method (instead of the Zeller's Rule).
*/
static inline uint8_t dayOfWeek(uint32_t year, uint8_t month, uint8_t day){

    uint32_t x = ((year % 100) / 4) + day;

    x += monthToValue(month);

    if(true == isYearLeap(year) && (month <= 1)) {
        x -= 1 ;
    }

    x += yearCode(year);
    x += (year %100);
    x = (x%7);

    /* Value of 1 means Sunday, value of 0 means Saturday */
    return((x)?(x-1):Sat);
}

int main(int argc, char* argv[]){

    time_t theTimeInSeconds;

    /* Retrieve the current time and date since the epoch */
    theTimeInSeconds = time(0);
    /* Convert the time in seconds to a struct tm */
    struct tm *theTime = localtime(&theTimeInSeconds);


    printf("The Current Time Since Epoch"
        " (1970-1-1, 00:00:00 +0000UTC) = %ld sec \n", theTimeInSeconds);


    if(0 == theTime){
        printf("The call to localtime() returned a null");
        return(-1);
    }

    printf("In YYYY-MM-DD, hh:mm:ss : %u-%d-%d, %d:%d:%d\n",
        yearInStructTm+theTime->tm_year,
        theTime->tm_mon +1,
        theTime->tm_mday,
        theTime->tm_hour,
        theTime->tm_min,
        theTime->tm_sec);

    uint32_t derivedTimeInSeconds = tmInSeconds(*theTime, true);
    printf("tmInSeconds() = %u\n", derivedTimeInSeconds);

    if(derivedTimeInSeconds != (uint32_t)theTimeInSeconds){
        printf("Retrieved time - time(), is not the same as "
            "derived time - tmInSeconds()");
    }

    leapYearChecker( leapYearGoldenTestTable,
        sizeof(leapYearGoldenTestTable)/sizeof(uint32_t),
        startYearRange, endYearRange);

    struct tm itsTime = {0};

    itsTime = secondsInStuctTm(max32BitVal);
    printf("Time in Sec: %u \n", max32BitVal);
    printStructTm(itsTime);

    itsTime = secondsInStuctTm(half32BitVal);
    printf("Time in Sec: %u \n", half32BitVal);
    printStructTm(itsTime);

    dayOfTheWeekChecker();

    return(0);
}


/*=============================================================================
 *
 * tmInSeconds: Converts a struct tm to seconds since Epoch.
 *
 * Converts a struct tm to seconds since Epoch. The value return by this
 * function should match https://www.unixtimestamp.com/index.php for the same
 * tm structure.
 *
 * Notes: N/A
 =============================================================================*/
uint32_t tmInSeconds(struct tm time, bool adjustForUTC){

    /* Current Year - 1970 */
    const uint32_t yearsInTotal = (time.tm_year + yearInStructTm)
        - epochYear;

    uint32_t timeInSeconds = 0;

    /* Count all the leap years since the epoch */
    uint32_t leapYearCount = 0;
    for(uint32_t i = 0; i < yearsInTotal; i++){
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
    timeInSeconds = ((secondsInYear * yearsInTotal) +
        (leapYearCount * secondsInDay));
    /* The time in seconds for all the days for the current year */
    timeInSeconds += (time.tm_yday * secondsInDay);
    /* The time in seconds for all the hours in the current day */
    timeInSeconds += (time.tm_hour * secondsInHour);
    /* The time in seconds for the minutes since the current hour */
    timeInSeconds += (time.tm_min * secondsInMin);
    /* The time in seconds from the current minute */
    timeInSeconds += time.tm_sec;
    /* Adjust the timezone for UTC */
    /*
     * if(true == adjustForUTC) {
     * timeInSeconds -= time.tm_gmtoff;
     * }
     */

    /* The value should match: https://www.unixtimestamp.com/index.php*/
    return(timeInSeconds);
}


struct tm secondsInStuctTm(const uint32_t timeInSeconds){
    struct tm theTime = {0};
    uint32_t remainingTimeInSec=0;
    uint32_t daysRemaining = 0;

    theTime.tm_year = (yearsSinceEpoch(timeInSeconds,
        &remainingTimeInSec) + 1970) - 1900;

    theTime.tm_yday = daysFromSec(remainingTimeInSec, &remainingTimeInSec);

    theTime.tm_mon = monthFromDayOfYear(theTime.tm_yday,
        &daysRemaining, isYearLeap(theTime.tm_year + 1900));
    theTime.tm_mday = daysRemaining;

    theTime.tm_hour = hoursFromSec(remainingTimeInSec, &remainingTimeInSec);
    theTime.tm_min = minsFromSec(remainingTimeInSec, &remainingTimeInSec);
    theTime.tm_sec = remainingTimeInSec;

    theTime.tm_wday = dayOfWeek((theTime.tm_year + 1900), theTime.tm_mon, theTime.tm_mday);

    return(theTime);
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


uint32_t monthFromDayOfYear(uint32_t dayOfYear, uint32_t* daysRemaining, bool isYearLeap){
    uint32_t i;
    uint32_t totalDayCount = 0;
    uint8_t monInDays = 0;
    uint32_t monthOfYear = 0;

    for(i=0; i<12; i++){

        monInDays = ((i==1) && (true==isYearLeap))?(monDayCount[i] +1):monDayCount[i];

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
/*=============================================================================
 *
 * isYearLeap: determines if the year passed in is leap or not.
 *
 * Formulae for determining Leap Years
 *
 * POSIX.1 defines seconds since the Epoch using a formula that approximates
 * the number of seconds between a specified time and the Epoch.
 * This formula takes account of the facts that all years that are evenly
 * divisible by 4 are leap years, but years that are evenly divisible by 100
 * are not leap years unless they are also evenly divisible by 400, in which
 * case they are leap years. This value is not the same as the actual number
 * of seconds between the time and the Epoch, because of leap seconds and
 * because system clocks are not required to be synchronized to a standard
 * reference. The intention is that the interpretation of seconds since the
 * Epoch values be consistent; see POSIX.1-2008 Rationale A.4.15 for further
 * rationale.
 *
 * Notes: N/A
 =============================================================================*/
bool isYearLeap(uint32_t year) {
    bool ret = false;

    /* Is this year divisible by 4 ? */
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

/*=============================================================================
 *
 * leapYearChecker: Tests the functionality of the isLeapYear() function.
 *
 * Tests the functionality of the isLeapYear() function by comparing a
 * well known table of leap years vs one that we build internally in this
 * function using the same year range. If the two tables match both in size
 * and content we return true, otherwise false.
 *
 * Notes: N/A
 *=============================================================================*/
bool leapYearChecker(const uint32_t* goldenLeapYearTable,
    const uint32_t goldenLeapYearTableSz,
    uint32_t startYear,
    uint32_t endYear){

    bool ret = false;

    /* End has to be higher than start and can't have a null pointer */
    if((endYear < startYear) || (0 == goldenLeapYearTable)){
        return(ret);
    }

    /* Set up the derived table */
    const uint32_t derivedLeapYearTableSz = ((endYear-startYear) + 1);
    uint32_t derivedLeapYearTable[derivedLeapYearTableSz];
    uint32_t derivedLeapYearTableIndex = 0;

    /* Print Golden Leap Year Table */
    printf("\n");
    printf("========== Printing Golden Table ==========\n");
    printf("Table Size = %u \n", goldenLeapYearTableSz);
    printTable(goldenLeapYearTable, goldenLeapYearTableSz, tableRowSize);
    printf("\n");

    /* Calculate and Print the Leap Year Table */
    for(uint32_t i = startYear; i <= endYear; i++){
        if(true == isYearLeap(i)){
            derivedLeapYearTable[derivedLeapYearTableIndex++] = i;
        }
    }

    printf("\n");
    printf("========== Printing Calculated Table ==========\n");
    printf("Table Size = %u \n", derivedLeapYearTableIndex);
    printTable(derivedLeapYearTable, derivedLeapYearTableIndex, tableRowSize);
    printf("\n");

    /* Size Check for the tables */
    if(goldenLeapYearTableSz != derivedLeapYearTableIndex){
        printf("Error: Golden Leap Table Size = %u Calculated Leap Year Table Size = %u\n",
            goldenLeapYearTableSz, derivedLeapYearTableIndex);

            ret = false;
    } else {
        ret = true;
    }

    /* Content Check for the tables */
    if(true == ret){
        if(0 != memcmp((const void*)goldenLeapYearTable,
            (const void*)derivedLeapYearTable,
            goldenLeapYearTableSz)) {
            printf("Error: Golden Leap Table doesn't match Calculated Leap Table\n");
            ret = false;
        } else {
            ret = true;
        }

    }

    return(ret);
}

bool dayOfTheWeekChecker(void){

    uint32_t i = 0;

    struct _simpleDate {
        uint32_t year;
        uint8_t month;
        uint8_t day;
        const char * const dayOfWeek;
    };

    const char* day;

    struct _simpleDate checkTable[] = {
        {2000, Feb, 28, "Monday"},
        {2000, Feb, 29, "Tuesday"},
        {2000, Mar, 1,  "Wednesday"},
        {1828, Feb, 29, "Friday"},
        {1620, Feb, 29, "Saturday"},
        {1976, Feb, 29, "Sunday"},
        {1970, Jan, 1,  "Thursday"}
    };

    for(i=0; i<7; i++){

        day = dayOfWeekToString(dayOfWeek(
            checkTable[i].year,
            checkTable[i].month,
            checkTable[i].day));

        if(0 != strcmp(day, checkTable[i].dayOfWeek)){
            printf("Failed: Year: %d, month: %s, day: %d, dayOfWeek: %s\n",
                checkTable[i].year,
                monthToStringAbrv(checkTable[i].month),
                checkTable[i].day,
                checkTable[i].dayOfWeek);
            break;
        } else {
             printf("Passed: Year: %d, month: %s, day: %d, dayOfWeek: %s\n",
                checkTable[i].year,
                monthToStringAbrv(checkTable[i].month),
                checkTable[i].day,
                checkTable[i].dayOfWeek);
        }
    }

    return(false);

}

/*=============================================================================
 *
 * printTable: Prints a table of data in tabular format
 *
 * Prints a table of data in tabular format taking in as parameters
 * the table itself, the size of the table, and the row length.
 *
 *
 * Notes: N/A
 *============================================================================*/
void printTable(const uint32_t* table, const uint32_t size,
    const uint8_t blockSize){

    if((0==table) || (0==size) || (0==blockSize))
        return;

    for(uint32_t i = 0; i < size; i++){
        if(0 == ((i+1) % (blockSize))) {
            printf("%d,\n", table[i]);
        } else {
            printf("%d, ", table[i]);
        }
    }
    printf("\n");
}


void printStructTm(struct tm theTime){
    printf("tm.year: %d\n", theTime.tm_year );
    printf("tm.year + 1900: %d\n", (theTime.tm_year + 1900));

    printf("tm.mon: %d (%s)\n", theTime.tm_mon,
       monthToString(theTime.tm_mon));

    printf("tm.yday: %d\n", theTime.tm_yday);
    printf("tm.mday: %d\n", theTime.tm_mday);
    printf("tm.hour: %d\n", theTime.tm_hour);
    printf("tm.min: %d\n", theTime.tm_min);
    printf("tm.sec: %d\n", theTime.tm_sec);

    printf("tm.wday: %d (%s)\n", theTime.tm_wday,
        dayOfWeekToString(theTime.tm_wday));

}

// Compile as g++ -Wall -g -o get_time get_time.c
// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#include "rtc_base.h"

struct tm secondsInTm(struct tm *timeStruct, unsigned long seconds);

bool leapYearChecker(const unsigned int* goldenLeapYearTable,
    const unsigned long goldenLeapYearTableSz,
    unsigned int startYear,
    unsigned int endYear);

void printTable(const unsigned int* table,
    const unsigned int tableSize,
    const unsigned char rowLength);


struct tm secondsInStuctTm(const unsigned long timeInSeconds);

static unsigned int yearsSinceEpoch(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec);

static unsigned int daysFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec);

static unsigned int hoursFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec);

static unsigned int minsFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec);

static unsigned int monthFromDayOfYear(const unsigned int dayOfYear,
    unsigned int* daysRemaining,
    bool isYearLeap);

static void printStructTm(const struct tm theTime);

const static unsigned long epochYear = 1970;
const static unsigned long secondsInDay = 86400;
const static unsigned long secondsInYear = 31536000;
const static unsigned long secondsInLeapYear = 31622400;
const static unsigned long secondsInHour = 3600;
const static unsigned long secondsInMin = 60;
const static unsigned long yearInStructTm = 1900;
const static unsigned long startYearRange = 1600;
const static unsigned long endYearRange = 2400;
const static unsigned long tableRowSize = 10;

const static unsigned long max32BitVal = 4294967295;
const static unsigned long half32BitVal = 2147483647;

// Leap Years from 1800-2400
// https://kalender-365.de/leap-years.php
// https://miniwebtool.com/leap-years-list/?start_year=1800&end_year=2400
static unsigned int leapYearGoldenTestTable[] = {
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


char* monStrings[] = {
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


char* monAbrvStrings[] = {
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

unsigned char monDayCount[] = {
    31,
    28, // Leap Year 29
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



int main(int argc, char* argv[]){

    time_t theTimeInSeconds;

    // Retrieve the current time and date since the epoch.
    theTimeInSeconds = time(0);
    // Convert the time in seconds to a struct tm
    struct tm *theTime = localtime(&theTimeInSeconds);


    printf("The Current Time Since Epoch"
        " (1970-1-1, 00:00:00 +0000UTC) = %ld sec \n", theTimeInSeconds);


    if(0 == theTime){
        printf("The call to localtime() returned a null");
        return(-1);
    }

    printf("In YYYY-MM-DD, hh:mm:ss : %lu-%d-%d, %d:%d:%d\n",
        yearInStructTm+theTime->tm_year,
        theTime->tm_mon +1,
        theTime->tm_mday,
        theTime->tm_hour,
        theTime->tm_min,
        theTime->tm_sec);

    unsigned long derivedTimeInSeconds = tmInSeconds(*theTime, true);
    printf("tmInSeconds() = %lu\n", derivedTimeInSeconds);

    if(derivedTimeInSeconds != (unsigned long)theTimeInSeconds){
        printf("Retrieved time - time(), is not the same as "
            "derived time - tmInSeconds()");
    }

    leapYearChecker( leapYearGoldenTestTable,
        sizeof(leapYearGoldenTestTable)/sizeof(unsigned int),
        startYearRange, endYearRange);

    struct tm itsTime = {0};

    itsTime = secondsInStuctTm(max32BitVal);
    printf("Time in Sec: %lu \n", max32BitVal);
    printStructTm(itsTime);

    itsTime = secondsInStuctTm(half32BitVal);
    printf("Time in Sec: %lu \n", half32BitVal);
    printStructTm(itsTime);


    return(0);
}


//=============================================================================
//
// tmInSeconds: Converts a struct tm to seconds since Epoch.
//
// Converts a struct tm to seconds since Epoch. The value return by this
// function should match https://www.unixtimestamp.com/index.php for the same
// tm structure.
//
// Notes: N/A
//=============================================================================
unsigned long tmInSeconds(struct tm time, bool adjustForUTC){
    //
    // Current Year - 1970
    //
    const unsigned long yearsInTotal = (time.tm_year + yearInStructTm)
        - epochYear;

    unsigned long timeInSeconds = 0;

    // Count all the leap years since the epoch
    unsigned int leapYearCount = 0;
    for(unsigned int i = 0; i < yearsInTotal; i++){
        unsigned int year = epochYear + i;
        // Is this a leap year ?
        if(isYearLeap(year)){
            ++leapYearCount;
        }
    }

    // The time in seconds for all the years since
    // 1970 + the additional leap days.
    timeInSeconds = ((secondsInYear * yearsInTotal) +
        (leapYearCount * secondsInDay));
    // The time in seconds for all the days for the current year.
    timeInSeconds += (time.tm_yday * secondsInDay);
    // The time in seconds for all the hours in the current day.
    timeInSeconds += (time.tm_hour * secondsInHour);
    // The time in seconds for the minutes since the current hour
    timeInSeconds += (time.tm_min * secondsInMin);
    // The time in seconds from the current minute.
    timeInSeconds += time.tm_sec;
    // Adjust the timezone for UTC.
    if(true == adjustForUTC) {
        timeInSeconds -= time.tm_gmtoff;
    }

    // The value should match: https://www.unixtimestamp.com/index.php
    return(timeInSeconds);
}


struct tm secondsInStuctTm(const unsigned long timeInSeconds){
    struct tm theTime = {0};
    unsigned long remainingTimeInSec=0;
    unsigned int daysRemaining = 0;

    theTime.tm_year = (yearsSinceEpoch(timeInSeconds,
        &remainingTimeInSec) + 1970) - 1900;

    theTime.tm_yday = daysFromSec(remainingTimeInSec, &remainingTimeInSec);

    theTime.tm_mon = monthFromDayOfYear(theTime.tm_yday,
        &daysRemaining, isYearLeap(theTime.tm_year + 1900));
    theTime.tm_mday = daysRemaining;

    theTime.tm_hour = hoursFromSec(remainingTimeInSec, &remainingTimeInSec);
    theTime.tm_min = minsFromSec(remainingTimeInSec, &remainingTimeInSec);
    theTime.tm_sec = remainingTimeInSec;

    return(theTime);
}

unsigned int yearsSinceEpoch(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec){

    unsigned int yearCount = 0;
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

unsigned int daysFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInDay;
    // Days don't start from 0
    return((timeInSeconds / secondsInDay)+1);
}

unsigned int hoursFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInHour;

    return(timeInSeconds / secondsInHour);
}

unsigned int minsFromSec(const unsigned long timeInSeconds,
    unsigned long *remainingTimeInSec) {

    *remainingTimeInSec= timeInSeconds % secondsInMin;

    return(timeInSeconds / secondsInMin);
}


unsigned int monthFromDayOfYear(unsigned int dayOfYear, unsigned int* daysRemaining, bool isYearLeap){
    unsigned int i;
    unsigned int totalDayCount = 0;
    unsigned char monInDays = 0;
    unsigned int monthOfYear = 0;

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
//=============================================================================
//
// isYearLeap: determines if the year passed in is leap or not.
//
// Formulae for determining Leap Years
//
// POSIX.1 defines seconds since the Epoch using a formula that approximates
// the number of seconds between a specified time and the Epoch.
// This formula takes account of the facts that all years that are evenly
// divisible by 4 are leap years, but years that are evenly divisible by 100
// are not leap years unless they are also evenly divisible by 400, in which
// case they are leap years. This value is not the same as the actual number
// of seconds between the time and the Epoch, because of leap seconds and
// because system clocks are not required to be synchronized to a standard
// reference. The intention is that the interpretation of seconds since the
// Epoch values be consistent; see POSIX.1-2008 Rationale A.4.15 for further
// rationale.
//
// Notes: N/A
//=============================================================================
bool isYearLeap(unsigned long year) {
    bool ret = false;

    // Is this year divisible by 4 ?
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

//=============================================================================
//
// leapYearChecker: Tests the functionality of the isLeapYear() function.
//
// Tests the functionality of the isLeapYear() function by comparing a
// well known table of leap years vs one that we build internally in this
// function using the same year range. If the two tables match both in size
// and content we return true, otherwise false.
//
// Notes: N/A
//=============================================================================
bool leapYearChecker(const unsigned int* goldenLeapYearTable,
    const unsigned long goldenLeapYearTableSz,
    unsigned int startYear,
    unsigned int endYear){

    bool ret = false;

    // End has to be higher than start and can't have a null pointer
    if((endYear < startYear) || (0 == goldenLeapYearTable)){
        return(ret);
    }

    // Set up the derived table
    const unsigned int derivedLeapYearTableSz = ((endYear-startYear) + 1);
    unsigned int derivedLeapYearTable[derivedLeapYearTableSz];
    unsigned long derivedLeapYearTableIndex = 0;

    // Print Golden Leap Year Table
    printf("\n");
    printf("========== Printing Golden Table ==========\n");
    printf("Table Size = %lu \n", goldenLeapYearTableSz);
    printTable(goldenLeapYearTable, goldenLeapYearTableSz, tableRowSize);
    printf("\n");

    // Calculate and Print the Leap Year Table
    for(unsigned int i = startYear; i <= endYear; i++){
        if(true == isYearLeap(i)){
            derivedLeapYearTable[derivedLeapYearTableIndex++] = i;
        }
    }

    printf("\n");
    printf("========== Printing Calculated Table ==========\n");
    printf("Table Size = %lu \n", derivedLeapYearTableIndex);
    printTable(derivedLeapYearTable, derivedLeapYearTableIndex, tableRowSize);
    printf("\n");

    // Size Check for the tables
    if(goldenLeapYearTableSz != derivedLeapYearTableIndex){
        printf("Error: Golden Leap Table Size = %lu Calculated Leap Year Table Size = %lu\n",
            goldenLeapYearTableSz, derivedLeapYearTableIndex);

            ret = false;
    } else {
        ret = true;
    }

    // Content Check for the tables
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

//=============================================================================
//
// printTable: Prints a table of data in tabular format
//
// Prints a table of data in tabular format taking in as parameters
// the table itself, the size of the table, and the row length.
//
//
// Notes: N/A
//=============================================================================
void printTable(const unsigned int* table, const unsigned int size,
    const unsigned char blockSize){

    if((0==table) || (0==size) || (0==blockSize))
        return;

    for(unsigned int i = 0; i < size; i++){
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
    printf("tm.mon: %d (%s)\n", theTime.tm_mon, monStrings[theTime.tm_mon]);
    printf("tm.yday: %d\n", theTime.tm_yday);
    printf("tm.mday: %d\n", theTime.tm_mday);
    printf("tm.hour: %d\n", theTime.tm_hour);
    printf("tm.min: %d\n", theTime.tm_min);
    printf("tm.sec: %d\n", theTime.tm_sec);
}
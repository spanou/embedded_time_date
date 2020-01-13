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

#include "../include/qltime.h"
#include "testvector.h"


#define TBLROW_SZ 10                /*Table Row Size For Printouts*/

extern TimeStampTestVector timeStampTestVectors[];

static const char* monStrings[] = {
    "January", "February", "March", "April","May", "June", "July", "August",
    "September", "October", "November", "December"
};

static const char* monAbrvStrings[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct","Nov",
    "Dec"
};

static const char* dayOfWeekStrings[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

bool leapYearChecker(const uint32_t* goldenLeapYearTable,
    const uint32_t goldenLeapYearTableSz,
    uint32_t startYear,
    uint32_t endYear);

void printTable(const uint32_t* table,
    const uint32_t tableSize,
    const uint8_t rowLength);

bool dayOfTheWeekChecker(void);

bool testTimeStampToStructTm(TimeStampTestVector* tv, size_t s);

bool compareTmStruct(struct tm lhs, struct tm rhs);

static void printStructTm(const struct tm theTime);

static INLINE const char* const monthToString(uint8_t month){
    return((month > 11)? NULL : monStrings[month]);
}

static INLINE const char* const monthToStringAbrv(uint8_t month){
    return((month > 11)? NULL : monAbrvStrings[month]);
}

static INLINE const char* const dayOfWeekToString(uint8_t dayOfWeek){
    return((dayOfWeek > 6)? NULL : dayOfWeekStrings[dayOfWeek]);
}


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

int main(int argc, char* argv[]){

    // time_t theTimeInSeconds;

    // /* Retrieve the current time and date since the epoch */
    // theTimeInSeconds = time(0);
    // /* Convert the time in seconds to a struct tm */
    // struct tm *theTime = localtime(&theTimeInSeconds);


    // printf("The Current Time Since Epoch"
    //     " (1970-1-1, 00:00:00 +0000UTC) = %ld sec \n", theTimeInSeconds);


    // if(0 == theTime){
    //     printf("The call to localtime() returned a null");
    //     return(-1);
    // }

    // printf("In YYYY-MM-DD, hh:mm:ss : %u-%d-%d, %d:%d:%d\n",
    //     YEAROFFSET_TM+theTime->tm_year,
    //     theTime->tm_mon +1,
    //     theTime->tm_mday,
    //     theTime->tm_hour,
    //     theTime->tm_min,
    //     theTime->tm_sec);

    // uint32_t derivedTimeInSeconds =0;

    // tmInSeconds(&derivedTimeInSeconds, *theTime);
    // printf("tmInSeconds() = %u\n", derivedTimeInSeconds);

    // if(derivedTimeInSeconds != (uint32_t)theTimeInSeconds){
    //     printf("Retrieved time - time(), is not the same as "
    //         "derived time - tmInSeconds()");
    // }

    leapYearChecker( leapYearGoldenTestTable,
        sizeof(leapYearGoldenTestTable)/sizeof(uint32_t),
        STARTYEAR_CODE_RANGE, ENDYEAR_CODE_RANGE);

    // struct tm itsTime = {0};

    // secondsInStuctTm(&itsTime, UINT32_MAX);
    // printf("Time in Sec: %u \n", UINT32_MAX);
    // printStructTm(itsTime);

    // secondsInStuctTm(&itsTime, (UINT32_MAX/2));
    // printf("Time in Sec: %u \n", (UINT32_MAX/2));
    // printStructTm(itsTime);

    // dayOfTheWeekChecker();

    //int vectorSize = 9;
    // int i = 0;

    // struct tm vectorTime;

    // for(i=0; i<vectorSize; i++){
    //     time_t theTime = testVectors[i].timeInSeconds;
    //     gmtime_r(&theTime, &vectorTime);

    //     printf("{%u,       {%d, %d, %d, %d, %d, %u, %d, %d, 0, 0, 0}}\n",
    //     testVectors[i].timeInSeconds,
    //     vectorTime.tm_sec,
    //     vectorTime.tm_min,
    //     vectorTime.tm_hour,
    //     vectorTime.tm_mday,
    //     vectorTime.tm_mon,
    //     vectorTime.tm_year,
    //     vectorTime.tm_wday,
    //     vectorTime.tm_yday);
    // }

    testTimeStampToStructTm(timeStampTestVectors, 10);

    return(0);
}

bool compareTmStruct(struct tm lhs, struct tm rhs){
    if(lhs.tm_sec != rhs.tm_sec)
        return(false);

    if(lhs.tm_min != rhs.tm_min)
        return(false);

    if(lhs.tm_hour != rhs.tm_hour)
        return(false);

    if(lhs.tm_mday != rhs.tm_mday)
        return(false);

    if(lhs.tm_mon != rhs.tm_mon)
        return(false);

    if(lhs.tm_year != rhs.tm_year)
        return(false);

    if(lhs.tm_wday != rhs.tm_wday)
        return(false);

    if(lhs.tm_yday != rhs.tm_yday)
        return(false);

    return(true);
}

bool testTimeStampToStructTm(TimeStampTestVector* tv, size_t s){
    size_t i = 0;
    Status st = NOERROR;
    uint32_t passCount = 0;

    if(tv == NULL){
        printf("(%d) %s:%s() param=tv was null\n",
            __LINE__, __FILE__, __FUNCTION__);
    }

    for(i=0; i<s; i++){
        struct tm derivedTm = {0};

        printf("(%d) %s:%s() Time Stamp Test #%lu: %u \n",
                    __LINE__, __FILE__, __FUNCTION__, i, tv[i].timeInSeconds);

        if(NOERROR == (st = secondsInStuctTm(&derivedTm, tv[i].timeInSeconds))){
            if(false == compareTmStruct(derivedTm, tv[i].timeStruct)){

                printf("(%d) %s:%s() Failed: Test Time Stamp \n",
                    __LINE__, __FILE__, __FUNCTION__);
                printf("****** Derived ******\n");
                printStructTm(derivedTm);
                printf("****** Expected ******\n");
                printStructTm(tv[i].timeStruct);

            } else {

                printf("(%d) %s:%s() Passed: Test Time Stamp \n",
                    __LINE__, __FILE__, __FUNCTION__);
                printStructTm(derivedTm);

                ++passCount;

            }
        } else {
            printf("(%d) %s:%s() Call to secondsInStuctTm() failed\n",
                __LINE__, __FILE__, __FUNCTION__);
        }
    }

    return((passCount!=s)?false:true);
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
    printTable(goldenLeapYearTable, goldenLeapYearTableSz, TBLROW_SZ);
    printf("\n");

    /* Calculate and Print the Leap Year Table */
    uint32_t i;
    for(i = startYear; i <= endYear; i++){
        if(true == isYearLeap(i)){
            derivedLeapYearTable[derivedLeapYearTableIndex++] = i;
        }
    }

    printf("\n");
    printf("========== Printing Calculated Table ==========\n");
    printf("Table Size = %u \n", derivedLeapYearTableIndex);
    printTable(derivedLeapYearTable, derivedLeapYearTableIndex, TBLROW_SZ);
    printf("\n");

    /* Size Check for the tables */
    if(goldenLeapYearTableSz != derivedLeapYearTableIndex){
        printf("Error: Golden Leap Table Size = %u Calculated Leap Year "
            "Table Size = %u\n",
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
            printf("Error: Golden Leap Table doesn't match Calculated Leap"
                " Table\n");
            ret = false;
        } else {
            ret = true;
        }

    }

    return(ret);
}

bool dayOfTheWeekChecker(void){

    uint32_t i = 0;
    uint8_t dayNum = 0;

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

        dayOfWeek(checkTable[i].year, checkTable[i].month, checkTable[i].day,
            &dayNum);
        day = dayOfWeekToString(dayNum);

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
    uint32_t i;
    for(i = 0; i < size; i++){
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

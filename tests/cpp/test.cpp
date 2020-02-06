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

#include "../include/qltime.h"
#include "../include/qllogger.h"
#include "testvector.h"


#define TBLROW_SZ 10                /*Table Row Size For Printouts*/

extern uint32_t timeStamps[];

// static const char* monStrings[] = {
//     "January", "February", "March", "April","May", "June", "July", "August",
//     "September", "October", "November", "December"
// };

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

void printTable(LogLevel level,
    const uint32_t* table,
    const uint32_t tableSize,
    const uint8_t rowLength);

bool dayOfTheWeekChecker(void);

bool testTimeStampToStructTm(void);
bool testStructTmToTimeStamp(void);

bool compareTmStruct(struct tm lhs, struct tm rhs);

static void printStructTm(LogLevel level, const struct tm theTime);

// static INLINE const char* const monthToString(uint8_t month){
//     return((month > 11)? NULL : monStrings[month]);
// }

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

    leapYearChecker( leapYearGoldenTestTable,
        sizeof(leapYearGoldenTestTable)/sizeof(uint32_t),
        STARTYEAR_CODE_RANGE, ENDYEAR_CODE_RANGE);

    testTimeStampToStructTm();

    testStructTmToTimeStamp();


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

bool testTimeStampToStructTm(void){
    uint32_t i = 0;
    Status st = NOERROR;
    uint32_t passCount = 0;
    uint32_t failCount = 0;
    uint32_t totalTestCount = UINT32_MAX / NSEC_MIN;
    uint32_t timeStamp = 0;

    qlLogger(ERROR, "Starting time stamp to struct tm tests...\n");

    for(i=0; i<totalTestCount; i++){

        struct tm derivedTm = {0};
        struct tm expectedTm = {0};

        time_t theTime = timeStamp;
        struct tm* bl = gmtime((const time_t*)&theTime);
        memcpy((void*)&expectedTm, (void*)bl, sizeof(struct tm));

        if(NOERROR == (st = secondsInStuctTm(&derivedTm, theTime))){

            if(false == compareTmStruct(derivedTm, expectedTm)){

                qlLogger(ERROR,
                    "(%d) %s::%s -> Failed at Time Stamp Test #%u: %lu \n",
                    __LINE__, __FILE__, __FUNCTION__, i, theTime);

                qlLogger(ERROR, "Derived => ");
                printStructTm(ERROR, derivedTm);
                qlLogger(ERROR, "Expected => ");
                printStructTm(ERROR, expectedTm);

                ++failCount;

            } else {

                qlLogger(INFO, "(%d) %s::%s -> Passed: Time Stamp Test #%u: %lu \n",
                    __LINE__, __FILE__, __FUNCTION__, i, theTime);

                ++passCount;
            }
        } else {

            qlLogger(ERROR, "(%d) %s::%s -> Call to secondsInStuctTm() failed\n",
                    __LINE__, __FILE__, __FUNCTION__);
            break;
        }

        timeStamp += NSEC_MIN;
    }

    qlLogger(ERROR, "Pass Count = %u, ", passCount);
    qlLogger(ERROR, "Fail Count = %u \n", failCount);

    return((failCount!=0)?false:true);
}


bool testStructTmToTimeStamp(void){
    uint32_t i = 0;
    Status st = NOERROR;
    uint32_t passCount = 0;
    uint32_t failCount = 0;
    uint32_t totalTestCount = UINT32_MAX / NSEC_MIN;
    uint32_t expectedTimeStamp = 0;

    qlLogger(ERROR, "Starting the struct tm to time stamp tests...\n");

    for(i=0; i<totalTestCount; i++){

        struct tm* theStructTm;
        time_t expectedTime = expectedTimeStamp;
        uint32_t derivedTmeStamp = 0;

        theStructTm = gmtime((const time_t*)&expectedTime);


        if(NOERROR == (st = tmInSeconds(&derivedTmeStamp, *theStructTm))){

            time_t derivedTime = derivedTmeStamp;

            if(derivedTime != expectedTime){

                qlLogger(ERROR,
                    "(%d) %s::%s -> Failed at Time Stamp Test #%u: %lu \n",
                    __LINE__, __FILE__, __FUNCTION__, i, expectedTime);

                qlLogger(ERROR, "Expected Time Stamp => %lu", expectedTime);
                printStructTm(ERROR, *theStructTm);

                qlLogger(ERROR, "Derived Time Stamp => %lu", derivedTime);

                ++failCount;

            } else {

                qlLogger(INFO, "(%d) %s::%s -> Passed: Time Stamp Test #%u: %lu \n",
                    __LINE__, __FILE__, __FUNCTION__, i, expectedTime);
                printStructTm(INFO, *theStructTm);

                ++passCount;
            }
        } else {

            qlLogger(ERROR, "(%d) %s::%s -> Call to tmInSeconds() failed\n",
                    __LINE__, __FILE__, __FUNCTION__);
            break;
        }

        expectedTimeStamp += NSEC_MIN;
    }

    qlLogger(ERROR, "Pass Count = %u, ", passCount);
    qlLogger(ERROR, "Fail Count = %u \n", failCount);

    return((failCount!=0)?false:true);
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

    qlLogger(ERROR,"Starting leap year tests...\n");

    /* Set up the derived table */
    const uint32_t derivedLeapYearTableSz = ((endYear-startYear) + 1);
    uint32_t derivedLeapYearTable[derivedLeapYearTableSz];
    uint32_t derivedLeapYearTableIndex = 0;

    /* Print Golden Leap Year Table */
    qlLogger(INFO, "\n");
    qlLogger(INFO,"========== Printing Golden Table ==========\n");
    qlLogger(INFO, "Table Size = %u \n", goldenLeapYearTableSz);
    printTable(INFO, goldenLeapYearTable, goldenLeapYearTableSz, TBLROW_SZ);
    qlLogger(INFO, "\n");

    /* Calculate and Print the Leap Year Table */
    uint32_t i;
    for(i = startYear; i <= endYear; i++){
        if(true == isYearLeap(i)){
            derivedLeapYearTable[derivedLeapYearTableIndex++] = i;
        }
    }

    qlLogger(INFO, "\n");
    qlLogger(INFO, "========== Printing Calculated Table ==========\n");
    qlLogger(INFO, "Table Size = %u \n", derivedLeapYearTableIndex);
    printTable(INFO, derivedLeapYearTable, derivedLeapYearTableIndex, TBLROW_SZ);
    qlLogger(INFO, "\n");

    /* Size Check for the tables */
    if(goldenLeapYearTableSz != derivedLeapYearTableIndex){
        qlLogger(ERROR,"Error: Golden Leap Table Size = %u Calculated Leap Year "
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
            qlLogger(ERROR,"Error: Golden Leap Table doesn't match Calculated Leap"
                " Table\n");
            ret = false;
        } else {
            qlLogger(ERROR,"Pass: Golden Leap Table matches the Calculated Leap"
                " Table\n");
            ret = true;
        }

    }

    return(ret);
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
void printTable(LogLevel level, const uint32_t* table, const uint32_t size,
    const uint8_t blockSize){

    if((0==table) || (0==size) || (0==blockSize))
        return;
    uint32_t i;
    for(i = 0; i < size; i++){
        if(0 == ((i+1) % (blockSize))) {
            qlLogger(level,"%d,\n", table[i]);
        } else {
            qlLogger(level,"%d, ", table[i]);
        }
    }
    qlLogger(level,"\n");
}


void printStructTm(LogLevel level, struct tm theTime){
    qlLogger(level,"tm.year: %d, ", theTime.tm_year );
    qlLogger(level,"tm.year + 1900: %d, ", (theTime.tm_year + 1900));

    qlLogger(level,"tm.mon: %d (%s), ", theTime.tm_mon,
        monthToStringAbrv(theTime.tm_mon));

    qlLogger(level,"tm.yday: %d, ", theTime.tm_yday);
    qlLogger(level,"tm.mday: %d, ", theTime.tm_mday);

    qlLogger(level,"tm.hour: %d, ", theTime.tm_hour);
    qlLogger(level,"tm.min: %d, ", theTime.tm_min);
    qlLogger(level,"tm.sec: %d, ", theTime.tm_sec);

    qlLogger(level,"tm.wday: %d (%s)", theTime.tm_wday,
        dayOfWeekToString(theTime.tm_wday));

    qlLogger(level,"\n");

}

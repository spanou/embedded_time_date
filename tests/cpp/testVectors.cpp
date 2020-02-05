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
#include "testvector.h"


// struct tm
// {
//   int tm_sec;            /* Seconds. [0-60] (1 leap second) */
//   int tm_min;            /* Minutes. [0-59] */
//   int tm_hour;           /* Hours.   [0-23] */
//   int tm_mday;           /* Day.     [1-31] */
//   int tm_mon;            /* Month.   [0-11] */
//   int tm_year;           /* Year - 1900.  */
//   int tm_wday;           /* Day of week. [0-6] */
//   int tm_yday;            Days in year.[0-365]
//   int tm_isdst;          /* DST.     [-1/0/1]*/

// # ifdef  __USE_MISC
//   long int tm_gmtoff;        /* Seconds east of UTC.  */
//   const char *tm_zone;       /* Timezone abbreviation.  */
// # else
//   long int __tm_gmtoff;      /* Seconds east of UTC.  */
//   const char *__tm_zone; /* Timezone abbreviation.  */
// # endif
// };

uint32_t timeStamps[] = {
    2678400,   // Sun, 01 Feb 1970 00:00:00
    86400,     // Fri, 02 Jan 1970 00:00:00 UTC
    0,         // Sun, 01 Feb 1970 00:00:00
    4294967295,// Sunday, 07-Feb-06 06:28:15 UTC
    2147483647,// Tuesday, 19-Jan-38 03:14:07 UTC
    1,         // Thursday, 01-Jan-70 00:00:01 UTC
    951782399, // Monday, 28-Feb-00 23:59:59 UTC
    951782400, // Tuesday, 29-Feb-00 00:00:00 UTC
    951782401, // Tuesday, 29-Feb-00 00:00:01 UTC
    194369040, // Saturday, 28-Feb-76 15:24:00 UTC
    946684799, // Friday, 31-Dec-99 23:59:59 UTC
    946684800, // Saturday, 01-Jan-00 00:00:00 UTC
    102444800, // Friday, 01-Jan-00 00:00:00 UTC
};

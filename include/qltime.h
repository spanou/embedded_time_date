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
#ifndef __QL_TIME_H__
#define __QL_TIME_H__

/*
 * [gcc -dM -E - < /dev/null] prints all the predefined macros
 * if -ansi or -std=c90, inline is not defined but __inline__ is.
 */
#ifdef __STDC_VERSION__
	#if(__STDC_VERSION__ >= 199901L)
		#define INLINE inline
	#else
		#define INLINE __inline__
	#endif
#else
	#define INLINE
#endif

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

enum monthsOfYear{
    Jan = 0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
};

enum daysOfWeek {
    Sun=0, Mon, Tue, Wed, Thu, Fri, Sat
};

typedef enum _Status {
	NOERROR = 0,
	INVLDPTR = 1
} Status;

/*=============================================================================
 *
 * isLeapYear: Determines if a year passed in is leap or not.
 *
 * year: 32 bit value that holds the year to be passed in this function.
 *
 * Return: returns true if year is leap, false otherwise.
 *
 * Notes:
 *  o - The value return must match https://www.unixtimestamp.com/index.php
 *
 *  o - This function assumes the caller will ensure atomicity for the durtion
 *      of its execution. i.e. it is not thread safe.
 *
 *  o - The forumulae for determining leap years is:
 *
 *      POSIX.1 defines seconds since the Epoch using a formula that appro-
 *      ximates the number of seconds between a specified time and the Epoch.
 *      This formula takes account of the facts that all years that are evenly
 *      divisible by 4 are leap years, but years that are evenly divisible by
 *      100 are not leap years unless they are also evenly divisible by 400,
 *      in which case they are leap years. This value is not the same as the
 *      actual number of seconds between the time and the Epoch, because of
 *      leap seconds and because system clocks are not required to be
 *      synchronized to a standard reference. The intention is that the
 *      interpretation of seconds since the Epoch values be consistent;
 *      see POSIX.1-2008 Rationale A.4.15 for further rationale.
 =============================================================================*/
extern bool isYearLeap(uint32_t year);

/*=============================================================================
 *
 * tmInSeconds: Converts a struct tm to seconds since Epoch (1970-1-1, 00:00:00)
 *
 * tmInSecs:    this pointer param will be set to the number of seconds
 *              specified by the value of the parameter 'time'.
 *
 * t:           The struct tm which holds the time to be converted to seconds
 *              since the Epoch.
 *
 *
 * Return: If successful it returns NOERROR(0) otherwise any other value.
 *
 * Notes:
 *  o - This function assumes the caller will ensure atomicity for the durtion
 *      of its execution. i.e. it is not thread safe.
 *
 =============================================================================*/
extern Status tmInSeconds(uint32_t *tmInSecs, struct tm t);

/*=============================================================================
 *
 * secondsInStuctTm: Converts seconds to a struct tm since Epoch
 *
 * t:        this struct tm pointer param will be populated with the right time
 *           field values based on the value of 'tmInSecs'
 *
 * tmInSecs: this 32 bit value represents the seconds lapsed since the Epoch
 *
 *
 * Return: If successful it returns NOERROR(0) otherwise any other value.
 *
 * Notes:
 *  o - Epoch is set at 1970-1-1, 00:00:00
 *
 *  o - This function assumes the caller will ensure atomicity for the durtion
 *      of its execution. i.e. it is not thread safe.
 *
 =============================================================================*/
extern Status secondsInStuctTm(struct tm *t, const uint32_t tmInSecs);

/*
 * http://mathforum.org/dr.math/faq/faq.calendar.html
 * Calculates the day of the week utilizing the Key Value Method (instead of the Zeller's Rule).
*/
extern uint8_t dayOfWeek(uint32_t year, uint8_t month, uint8_t day);
#endif
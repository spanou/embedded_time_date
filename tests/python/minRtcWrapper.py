#=============================================================================
# Copyright (c) 2019, Sakis Panou <sakis.panou@gmail.com>
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the <organization> nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#=============================================================================
from minRtcPythonCffiWrapper import ffi,lib


dayCodeInString = {
	0: "Sunday",
	1: "Monday",
	2: "Tuesday",
	3: "Wednesday",
	4: "Thursday",
	5: "Friday",
	6: "Saturday"
}

dayStringInCode = {
	"Sunday" : 0,
	"Monday" : 1,
	"Tuesday" :2,
	"Wednesday": 3,
	"Thursday": 4,
	"Friday": 5,
	"Saturday": 6
}

monthCodeInString = {
  	 0: "Jan",
  	 1: "Feb",
  	 2: "Mar",
  	 3: "Apr",
  	 4: "May",
  	 5: "Jun",
  	 6: "Jul",
  	 7: "Aug",
  	 8: "Sep",
  	 9: "Oct",
  	 10: "Nov",
  	 11: "Dec"
}

monthStringInCode = {
  	"Jan": 0,
  	"Feb": 1,
  	"Mar": 2,
  	"Apr": 3,
  	"May": 4,
  	"Jun": 5,
  	"Jul": 6,
  	"Aug": 7,
  	"Sep": 8,
  	"Oct": 9,
  	"Nov": 10,
  	"Dec": 11
}

def isLeapYear(year) :
  return lib.isYearLeap(year)

def dayOfWeekFromDate(year, month, day):
  dayCode = ffi.new("uint8_t*", 0)
  dayString = ""

  if 0 == lib.dayOfWeek(year, monthStringInCode[month], day, dayCode) :
    dayString = dayCodeInString[dayCode[0]]
  else:
    dayString = "Error"

  ffi.release(dayCode)

  return(dayString)


def secondsInStuctTm() :
  tm = ffi.new("struct tm*")

  print("Calling lib.secondsInStuctTm(tm, " + str(194369040) + " )")
  if 0 == lib.secondsInStuctTm(tm, 194369040) :
    print("Pass")
    print("tm.tm_sec = ", tm.tm_sec) 
    print("tm.tm_min = ", tm.tm_min)
    print("tm.tm_hour = ", tm.tm_hour)
    print("tm.tm_mday = ", tm.tm_mday)
    print("tm.tm_mon = ", tm.tm_mon )
    print("tm.tm_year = ", tm.tm_year)
    print("tm.tm_wday = ", tm.tm_wday )
    print("tm.tm_yday = ", tm.tm_yday )
    print("tm.tm_isdst = ", tm.tm_isdst )
    print("tm.tm_gmtoff = ", tm.tm_gmtoff )

  else:
    print("Fail")

  ffi.release(tm)
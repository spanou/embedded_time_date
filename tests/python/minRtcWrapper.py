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
from time import struct_time, time, localtime


dayCodeInString = {
	0: "Sun",	1: "Mon",	2: "Tue",	3: "Wed",
	4: "Thu",	5: "Fri",	6: "Sat" }

dayStringInCode = {
	"Sun": 0, "Mon": 1, "Tue": 2, "Wed": 3,
	"Thu": 4, "Fri": 5,	"Sat": 6 }

monthCodeInString = {
  0: "Jan", 1: "Feb", 2: "Mar",
  3: "Apr", 4: "May", 5: "Jun",
  6: "Jul", 7: "Aug", 8: "Sep",
  9: "Oct", 10: "Nov",11: "Dec" }

monthStringInCode = {
  "Jan": 0, "Feb": 1,	"Mar": 2,
  "Apr": 3,	"May": 4,	"Jun": 5,
  "Jul": 6,	"Aug": 7,	"Sep": 8,
  "Oct": 9,	"Nov": 10,"Dec": 11 }


class StructTmUnix :

  hour = None
  min = None
  sec = None
  mday = None
  mon = None
  year = None
  wday = None
  yday = None
  isdst = None
  gmtoff = None

  def __eq__(self, rhs):
    if(self.hour != rhs.hour):
      return(False)
    if(self.min != rhs.min):
      return(False)
    if(self.sec != rhs.sec):
      return(False)
    if(self.mday != rhs.mday):
      return(False)
    if(self.mon != rhs.mon):
      return(False)
    if(self.year != rhs.year):
      return(False)
    if(self.wday != rhs.wday):
      return(False)
    if(self.yday != rhs.yday):
      return(False)
    if(self.isdst != rhs.isdst):
      return(False)
    # if(self.gmtoff != rhs.gmtoff):
    #   return(False)
    #if we got to this point then the
    #two objects are equal
    return(True)

  def __ne__(self, rhs):
    return( not __eq__(self, rhs))


  def __init__(self, tm=None):

    if None == tm:
      tm = localtime(time())

    if type(tm) == StructTmUnix:
      self.hour = tm.hour
      self.min = tm.min
      self.sec = tm.sec
      self.mday = tm.mday
      self.mon = tm.mon
      self.year = tm.year
      self.wday = tm.wday
      self.yday = tm.yday
      self.isdst = tm.isdst
      self.gmtoff = tm.gmtoff

    if type(tm) == struct_time:
      self.hour = tm.tm_hour
      self.min = tm.tm_min
      self.sec = tm.tm_sec
      self.mday =  tm.tm_mday
      self.mon = tm.tm_mon-1
      self.year = tm.tm_year
      self.wday = 0 if tm.tm_wday==6 else tm.tm_wday+1
      self.yday = tm.tm_yday-1
      self.isdst = tm.tm_isdst
      self.gmtoff = tm.tm_gmtoff

  def __str__(self):
      return("{wday}, {mday:0>2} {ymon} {year:0>4} {hour:0>2}:{min:0>2}:{sec:0>2}".format(
        wday=dayCodeInString[self.wday],
        mday=str(self.mday),
        ymon=monthCodeInString[self.mon],
        year=str(self.year),
        hour=str(self.hour),
        min=str(self.min),
        sec=str(self.sec)))

  def __repr__(self):
      return("{wday}, {mday:0>2} {ymon} {year:0>4} {hour:0>2}:{min:0>2}:{sec:0>2}".format(
        wday=dayCodeInString[self.wday],
        mday=str(self.mday),
        ymon=monthCodeInString[self.mon],
        year=str(self.year),
        hour=str(self.hour),
        min=str(self.min),
        sec=str(self.sec)))

  def toStructTmUnix(self, cdata=None):

    if cdata == None:
      return None

    self.hour = cdata.tm_hour
    self.min = cdata.tm_min
    self.sec = cdata.tm_sec
    self.mday = cdata.tm_mday
    self.mon = cdata.tm_mon
    self.year = cdata.tm_year + 1900
    self.wday = cdata.tm_wday
    self.yday = cdata.tm_yday
    self.isdst = cdata.tm_isdst
    #self.gmtoff = cdata.gmtoff
    return self

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


def secondsInStuctTm(secsSinceEpoch=0) :
  newTm = StructTmUnix()
  tm = ffi.new("struct tm*")

  theType = type(tm)

  print("Calling lib.secondsInStuctTm(tm, " + str(secsSinceEpoch) + " )")
  if 0 == lib.secondsInStuctTm(tm, secsSinceEpoch) :
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
    newTm.toStructTmUnix(tm)

  else:
    print("Fail")

  ffi.release(tm)

  return newTm
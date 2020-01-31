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
import minRtcWrapper as rtcLib
from time import struct_time, gmtime, time, localtime


def testRtcLib():
	"""
		Exercises the following functions from minRtcWrapper :
			tmInSeconds(...)
			secondsInStuctTm(...);
			isYearLeap(...)
			dayOfWeek(...)
	"""
	testSecondsInStructTm()
	testTmInSeconds()
	testDayOfWeek()
	testIsYearLeap()

def testSecondsInStructTm():
	"""
		Test Every Hour to the minute and past the minute from the EPOCH to the
		maximum 32 bit unsigned integer. The struct_tm returned should match the
		one we get from the time module. If we get an error print out the test
		parameters and break execution.
	"""
	structTm = rtcLib.StructTmUnix()

def main():
	testRtcLib()

	# if rtcLib.isLeapYear(2000) :
	# 	print("Year 2000 is Leap")

	# print("Year: 2000, Month: Jan, Day: 1 -> ( " +
	# 	rtcLib.dayOfWeekFromDate(2000, "Jan", 1) + " )")

	# rtcLib.secondsInStuctTm()

	# itsTime1 = rtcLib.StructTmUnix()
	# itsTime1.year = 2000
	# itsTime1.mon = 0
	# itsTime1.mday = 1
	# itsTime1.hour = 0
	# itsTime1.min = 0
	# itsTime1.sec = 0
	# itsTime1.wday = 6
	# itsTime1.yday = 1

	# itsTime2 = rtcLib.StructTmUnix(itsTime1)

	# print("itsTime1 = " + str(itsTime1))
	# print("itsTime2 = " + str(itsTime2)) 

	# itsTime3 = rtcLib.StructTmUnix(time="Blah Blah")

	# itsTime4 = localtime(time())
	# itsTime5 = rtcLib.StructTmUnix(time=itsTime4)
	# print("itsTime5 = " + str(itsTime5)) 


if __name__ == '__main__':
	main()
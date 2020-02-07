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
import testInfrastructure as testInfra
from time import struct_time, gmtime, time, localtime, sleep



class testSecondsToStructTm(testInfra.Test):
	this = None
	testStatus = None

	def __init__(self, testStatus:testInfra.TestStatusProducer):
		self.this = self.__class__.__name__
		self.testStatus = testStatus

	def doTest(self):
		self.testStatus.setTestStatus("Started")
		count = 10000000

		for n in range(0, count):
			testData = {}
			testData["lbound"] = 0
			testData["ubound"] = count
			testData["index"] = n

			self.testStatus.setTestData(testData)

		self.testStatus.setTestStatus("Stopped")
		return

def testRtcLib():
	"""
		Exercises the following functions from minRtcWrapper :
			tmInSeconds(...)
			secondsInStuctTm(...);
			isYearLeap(...)
			dayOfWeek(...)
	"""
	#testSecondsInStructTm()
	testStructTmInSeconds()
	# testDayOfWeek()
	# testIsYearLeap()

def testSecondsInStructTm():
	"""
		Test Every Hour to the minute and past the minute from the EPOCH to the
		maximum 32 bit unsigned integer. The struct_tm returned should match the
		one we get from the time module. If we get an error print out the test
		parameters and break execution.
	"""
	secsSinceEpoch = 0
	#secsInHour = 3600
	secsInMinute = 60
	max32BitValue = 2**32
	passCount = 0
	failCount = 0

	while secsSinceEpoch < max32BitValue :
		structTmExpected = rtcLib.StructTmUnix(gmtime(secsSinceEpoch))
		structTmCalculated = rtcLib.secondsInStuctTm(secsSinceEpoch)

		if structTmCalculated != structTmExpected :
			print("Failed at {secCount:0>20} - Expected: {exp} Calculated: {calc}".format(
				secCount=secsSinceEpoch, exp=str(structTmExpected), calc=str(structTmCalculated)))
			print("Cal = " + structTmCalculated.debugPrint())
			print("Exp = " + structTmExpected.debugPrint())
			failCount += 1
		else:
			print("{secCount:0>20} - Expected: {exp} Calculated: {calc}".format(
				secCount=secsSinceEpoch, exp=str(structTmExpected), calc=str(structTmCalculated)))
			passCount += 1

		secsSinceEpoch += secsInMinute

	print("Pass Count : " + str(passCount) + " Fail Count : " + str(failCount))

def testStructTmInSeconds():
	secsSinceEpoch = 0
	secsInMinute = 60
	max32BitValue = 2**32
	passCount = 0
	failCount = 0

	print("Starting testStructTmInSeconds()")
	print("Max 32 Bit Value: " + str(max32BitValue))
	print("Itterations: " + str(max32BitValue/secsInMinute))

	while secsSinceEpoch < max32BitValue :
		unixTm = rtcLib.StructTmUnix(gmtime(secsSinceEpoch))
		secs = rtcLib.structTmUnixToSecs(unixTm)

		if(secs != secsSinceEpoch):
			print("Failed at {exp:0>20}, returned {calc:0>20}".format( exp=str(secsSinceEpoch), calc=str(secs)))
			print(str(unixTm))
			failCount += 1
		else:
			print("Passed at {exp:0>20}".format(exp=str(secsSinceEpoch)))
			passCount +=1

		secsSinceEpoch += secsInMinute

	print("Pass Count : " + str(passCount) + " Fail Count : " + str(failCount))


def main():
	#testRtcLib()

	testStatus = testInfra.TestStatus()
	testList = list()
	testList.append(testStatus)

	displayMonitor = testInfra.DisplayMonitor(testList)
	displayMonitor.start()

	test1 = testSecondsToStructTm(testStatus)
	test1.doInit()
	test1.doTest()
	test1.doCleanUp()

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
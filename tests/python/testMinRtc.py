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
    testName = None
    testStatus = None

    def __init__(self, testStatus:testInfra.TestStatusProducer):
        self.testName = self.__class__.__name__
        self.testStatus = testStatus

    def doTest(self):

        """
        Test Every Hour to the minute and past the minute from the EPOCH to the
        maximum 32 bit unsigned integer. The struct_tm returned should match the
        one we get from the time module. If we get an error print out the test
        parameters and break execution.
        """
        secsSinceEpoch = 0
        secsInMinute = 60
        max32BitValue = 2**32
        passCount = 0
        failCount = 0
        count = round(max32BitValue/secsInMinute)

        self.testStatus.setTestStatus("Test Name : " + self.testName + " Status -> [Started]")

        for n in range(0, count):
            testData = {}
            testData["lbound"] = 0
            testData["ubound"] = count
            testData["index"] = n

            self.testStatus.setTestData(testData)

            structTmExpected = rtcLib.StructTmUnix(gmtime(secsSinceEpoch))
            structTmCalculated = rtcLib.secondsInStuctTm(secsSinceEpoch)

            if structTmCalculated != structTmExpected :
                # print("Failed at {secCount:0>20} - Expected: {exp} Calculated: {calc}".format(
                #   secCount=secsSinceEpoch, exp=str(structTmExpected), calc=str(structTmCalculated)))
                # print("Cal = " + structTmCalculated.debugPrint())
                # print("Exp = " + structTmExpected.debugPrint())
                failCount += 1
            else:
                # print("{secCount:0>20} - Expected: {exp} Calculated: {calc}".format(
                #   secCount=secsSinceEpoch, exp=str(structTmExpected), calc=str(structTmCalculated)))
                passCount += 1

            secsSinceEpoch += secsInMinute

        self.testStatus.setTestStatus("Test Name : " + self.testName + " Status -> [Stopped]\n" +
            "Passed: " + str(passCount) + " Failed: " + str(failCount))

        return

class testStructTmToSeconds(testInfra.Test):
    testName = None
    testStatus = None

    def __init__(self, testStatus:testInfra.TestStatusProducer):
        self.testName = self.__class__.__name__
        self.testStatus = testStatus

    def doTest(self):
        secsSinceEpoch = 0
        secsInMinute = 60
        max32BitValue = 2**32
        passCount = 0
        failCount = 0
        count = round(max32BitValue/secsInMinute)

        self.testStatus.setTestStatus("Test Name : " + self.testName + " Status -> [Started]")

        for n in range(0, count):
            testData = {}
            testData["lbound"] = 0
            testData["ubound"] = count
            testData["index"] = n

            self.testStatus.setTestData(testData)

            unixTm = rtcLib.StructTmUnix(gmtime(secsSinceEpoch))
            secs = rtcLib.structTmUnixToSecs(unixTm)

            if(secs != secsSinceEpoch):
                # print("Failed at {exp:0>20}, returned {calc:0>20}".format( exp=str(secsSinceEpoch), calc=str(secs)))
                # print(str(unixTm))
                failCount += 1
            else:
                # print("Passed at {exp:0>20}".format(exp=str(secsSinceEpoch)))
                passCount +=1

            secsSinceEpoch += secsInMinute


        self.testStatus.setTestStatus("Test Name : " + self.testName + " Status -> [Stopped]\n" +
            "Passed: " + str(passCount) + " Failed: " + str(failCount))

        return

def main():
    testStatus1 = testInfra.TestStatus()
    testStatus2 = testInfra.TestStatus()
    testListStatuses = list()
    testListStatuses.append(testStatus1)
    testListStatuses.append(testStatus2)

    displayMonitor = testInfra.DisplayMonitor(testListStatuses)
    displayMonitor.start()

    testList = []
    testList.append(testSecondsToStructTm(testStatus1))
    testList.append(testStructTmToSeconds(testStatus2))

    for test in testList:
        test.doInit()
        test.doTest()
        test.doCleanUp()


if __name__ == '__main__':
    main()
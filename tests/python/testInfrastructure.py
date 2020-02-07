#==============================================================================
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
#==============================================================================
from threading import Thread
from time import sleep
import subprocess as sp

class TestStatusConsumer :
    '''(NULL)'''
    def __init__(self) :
        pass
    def getTestData (self) :
        # returns
        pass
    def getTestStatus (self) :
        # returns
        pass

class TestStatusProducer :
    '''(NULL)'''
    def __init__(self) :
        pass
    def setTestData (self) :
        # returns
        pass
    def setTestStatus (self) :
        # returns
        pass

class DisplayMonitor (Thread) :
    '''(NULL)'''
    testsList = None

    def __init__(self, testsList=None) :
        super().__init__(name="DisplayMonitor")
        self.testsList = testsList

    def run (self) :

        runState = True

        while runState :
            tmp = sp.call("clear", shell=True)

            for testConsumer in self.testsList :
                if True == self.stopDisplay(self.testsList) :
                    runState = False

                sts = testConsumer.getTestStatus()
                testData = testConsumer.getTestData()

                if(None != sts):
                    print(sts + "\n")

                if(None != testData):
                    ubound = testData["ubound"]
                    lbound = testData["lbound"]
                    index = testData["index"]

                    cur = int((index/ubound) * 100) + 1 #Round Up
                    remainder = int(100 - cur)
                    print(chr(9608)*cur + chr(9617)*remainder + " " + str(cur) + "% - " + str(index) + "\n")

                sleep(1)


    def stopDisplay(self, testList):
        testCount = len(testList)
        testStopped = 0
        for n in testList:
            status = n.getTestStatus()
            if(None != status):
                if(-1 != status.find("Stopped")):
                    testStopped += 1

        if(testCount == testStopped):
            return True

        return False



class TestStatus (TestStatusConsumer, TestStatusProducer) :
    '''(NULL)'''
    testData = None
    testStatus = None

    def __init__(self) :
        pass

    def getTestStatus (self) :
        return self.testStatus

    def setTestStatus (self, testStatus) :
        self.testStatus = testStatus

    def setTestData (self, testData) :
        self.testData = testData

    def getTestData (self) :
        return self.testData


class Test :
    '''(NULL)'''
    def __init__(self) :
        pass
    def doInit (self) :
        # returns
        pass
    def doTest (self) :
        # returns
        pass
    def doCleanUp (self) :
        # returns
        pass

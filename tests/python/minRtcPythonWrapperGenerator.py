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

import sys
from cffi import FFI
ffibuilder = FFI()


def doSetuUp(includeDirs, libName, libDir):
	ffibuilder.cdef("""
		    typedef enum _Status {
			    NOERROR = 0,
			    INVLDPTR = 1
			} Status;

			struct tm
			{
			  int tm_sec;                   /* Seconds.     [0-60] (1 leap second) */
			  int tm_min;                   /* Minutes.     [0-59] */
			  int tm_hour;                  /* Hours.       [0-23] */
			  int tm_mday;                  /* Day.         [1-31] */
			  int tm_mon;                   /* Month.       [0-11] */
			  int tm_year;                  /* Year - 1900.  */
			  int tm_wday;                  /* Day of week. [0-6] */
			  int tm_yday;                  /* Days in year.[0-365] */
			  int tm_isdst;                 /* DST.         [-1/0/1]*/
			  long int tm_gmtoff;         /* Seconds east of UTC.  */
			  const char *tm_zone;        /* Timezone abbreviation.  */
			};


			bool isYearLeap(uint32_t year);
			Status tmInSeconds(uint32_t *tmInSecs, struct tm t);
			Status secondsInStuctTm(struct tm *t, const uint32_t tmInSecs);
			Status dayOfWeek(uint32_t year, uint8_t month, uint8_t day, uint8_t* wday);

		""")
	# TODO: This is an erroneous approach, if the user passes more than one
	# library paths, position 0 can have any of them.
	linkArgs= [""]
	linkArg0 = "-Wl,-rpath=" + libDir[0]
	linkArgs[0] = linkArg0

	ffibuilder.set_source("minRtcPythonCffiWrapper",
		"""
			 #include <stdio.h>
			 #include <time.h>
			 #include <string.h>
			 #include <stdint.h>
			 #include <stdbool.h>
		     #include "qltime.h"
		""",

		libraries=libName,
		library_dirs=libDir,
		include_dirs=includeDirs,
		extra_link_args=linkArgs)

if __name__ == "__main__":

	if(len(sys.argv) < 4):
		print("""
			Error, this script expects 3 positional arguments, these are:
				Arg[0] = Include Directories Path
				Arg[1] = Library Linker Name
				Arg[2] = Library Directory
			""")
		sys.exit(0)
	else:
		for s in sys.argv:
			print(s)

		incDirs = [""]
		incDirs[0] = sys.argv[1]
		libNames = [""]
		libNames[0] = sys.argv[2]
		libDirs = [""]
		libDirs[0] = sys.argv[3]

		doSetuUp(includeDirs=incDirs, libName=libNames, libDir=libDirs)

	ffibuilder.compile(verbose=True)


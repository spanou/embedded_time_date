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

from cffi import FFI
ffibuilder = FFI()

# cdef() expects a single string declaring the C types, functions and
# globals needed to use the shared object. It must be in valid C syntax.
ffibuilder.cdef("""
    typedef enum _Status {
	    NOERROR = 0,
	    INVLDPTR = 1
	} Status;

	bool isYearLeap(uint32_t year);
	Status tmInSeconds(uint32_t *tmInSecs, struct tm t);
	Status secondsInStuctTm(struct tm *t, const uint32_t tmInSecs);
	Status dayOfWeek(uint32_t year, uint8_t month, uint8_t day, uint8_t* wday);

""")

# set_source() gives the name of the python extension module to
# produce, and some C source code as a string.  This C code needs
# to make the declarated functions, types and globals available,
# so it is often just the "#include".
ffibuilder.set_source("_min_rtc_cffi",
"""
	 #include<stdio.h>
	 #include<time.h>
	 #include<string.h>
	 #include<stdint.h>
	 #include<stdbool.h>
     #include "qltime.h"
""",

#TODO": remove all hardcoded paths
     libraries=['min_rtc'],
     library_dirs=['/scratch/development/experimental/rtc/tests/cpp/../../lib/'],
     include_dirs=['../../lib/include'],
     extra_link_args=["-Wl,-rpath=/scratch/development/experimental/rtc/tests/cpp/../../lib/"])   # library name, for the linker

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
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
     #include "include/qltime.h"   // the C header of the library
""",
     libraries=['min_rtc'])   # library name, for the linker

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
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

LIB_RTC_NAME=min_rtc
LIB_RTC_VER_MAJ=1
LIB_RTC_VER_MIN=0
LIB_RTC_VER_REL=0
LIB_RTC_PREFIX=lib

LIB_RTC_LINKER=$(LIB_RTC_PREFIX)$(LIB_RTC_NAME).so
LIB_RTC_SONAME=$(LIB_RTC_LINKER).$(LIB_RTC_VER_MAJ)
LIB_RTC_REAL= $(LIB_RTC_SONAME).$(LIB_RTC_VER_MIN).$(LIB_RTC_VER_REL)

LN?=ln
LDCONFIG?=ldconfig

ifeq (0, ${MAKELEVEL})
	CURDIR?=$(PWD)
else
	CURDIR?=$(PWD)/$(SUBDIR)
endif

OSFLAG :=

ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		OSFLAG += -D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			OSFLAG += -D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			OSFLAG += -D IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		OSFLAG += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		OSFLAG += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		OSFLAG += -D ARM
	endif
endif

vpath= src/ include/

# LOG_LEVEL values:
#  OFF=0
#  LOW=1,
#  INFO=2,
#  WARNING=4,
#  ERROR=8,
#  CRITICAL=16
LOGL?= 0

# LOG_MODE values:
#  FLAG=0
#  LEVEL=1
LOGM?= 1


CC= gcc
CFLAGS=-Wall -g -I ./include -I ../include -std=c90 -D LOG_LEVEL=$(LOGL) -D LOG_MODE=$(LOGM)
C_SRC?= $(wildcard ./src/*.c)

LFLAGS= -Wl,-rpath=. -o

OBJS_LIB= $(C_SRC:.c=.o)
HEADERS_LIB= $(wildcard ./include/*.h)
LIB_FLAGS= rcs


DEPS= $(C_SRC:.c=.d)

%.o:%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

%.d:%.c
	$(CC) $(CFLAGS) -M -MF"$@" $<


$(LIB_RTC_LINKER): $(LIB_RTC_SONAME)
	ln -sf $< $@

$(LIB_RTC_SONAME): $(LIB_RTC_REAL)
	$(LDCONFIG) -n $(CURDIR)


$(LIB_RTC_REAL): $(OBJS_LIB) $(DEPS) $(HEADERS_LIB)
	$(CC) -shared -Wl,-h,$(LIB_RTC_SONAME) -o $@ $(OBJS_LIB)

all: $(LIB_RTC_LINKER)

.PHONY: printinfo
printinfo:
	@echo "Compiler Pre Defines ..........."
	$(CC) -dM -E - < /dev/null
	@echo "CFLAGS ........................."
	@echo $(CFLAGS)
	@echo "LFLAGS ........................."
	@echo $(LFLAGS)
	@echo "AR.............................."
	@echo $(AR)
	@echo "LN.............................."
	@echo $(LN)
	@echo "OSFLAG.........................."
	@echo $(OSFLAG)
	@echo "LDCONFIG........................"
	@echo $(LDCONFIG)
	@echo "PWD............................"
	@echo $(PWD)


.PHONY: clean
clean:
	$(RM) *.o ./src/*.o ./src/*.d
	$(RM) $(LIB_RTC_REAL) $(LIB_RTC_SONAME) $(LIB_RTC_LINKER)

.PHONY: rebuild
rebuild: clean all


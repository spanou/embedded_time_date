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
EXECUTABLE=rtc_validation
LIB_RTC=min_rtc
STATIC_LIB=lib$(LIB_RTC).a


VPATH= src/ tests/ include/

# LOG_LEVEL values:
#
#  NONE=1,
#  INFO=2,
#  WARNING=4,
#  ERROR=8,
#  CRITICAL=16
LOGL?= 1

# LOG_MODE values:
#  FLAG=0
#  LEVEL=1
LOGM?= 0

CXX= g++
CXXFLAGS= -Wall -std=c++11 -g -I ./include -I ../include -D LOG_LEVEL=$(LOGL) -D LOG_MODE=$(LOGM)
CPP_SRC?= $(wildcard ./tests/*.cpp)

CC= gcc
CFLAGS=-Wall -g -I ./include -I ../include -std=c90 -D LOG_LEVEL=$(LOGL) -D LOG_MODE=$(LOGM)
C_SRC?= $(wildcard ./src/*.c)

LFLAGS= -o

OBJS_LIB= $(C_SRC:.c=.o)
HEADERS_LIB= $(wildcard ./include/*.h)
LIB_FLAGS= rcs

OBJS= $(CPP_SRC:.cpp=.o)
DEPS= $(CPP_SRC:.cpp=.d) $(C_SRC:.c=.d)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d:%.c
	$(CC) $(CFLAGS) -M -MF"$@" $<

%.d:%.cpp
	$(CXX) $(CXXFLAGS) -M -MF"$@" $<


$(EXECUTABLE): $(OBJS) $(DEPS) $(STATIC_LIB)
	$(CXX) $(OBJS) $(LFLAGS) $@ -L. -l$(LIB_RTC)

$(STATIC_LIB): $(OBJS_LIB) $(HEADERS_LIB)
	$(AR) $(LIB_FLAGS) $@ $(OBJS_LIB)

all: $(EXECUTABLE) $(STATIC_LIB)

.PHONY: printinfo
printinfo:
	@echo "Compiler Pre Defines ..........."
	$(CC) -dM -E - < /dev/null
	@echo "CXXFLAGS ......................."
	@echo $(CXXFLAGS)
	@echo "CFLAGS ........................."
	@echo $(CFLAGS)
	@echo "LFLAGS ........................."
	@echo $(LFLAGS)
	@echo "AR.... ........................."
	@echo $(AR)

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) *.o ./tests/*.o ./src/*.o ./tests/*.d ./src/*.d *.a

.PHONY: rebuild
rebuild: clean all

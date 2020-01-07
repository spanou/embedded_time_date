EXECUTABLE=rtc_validation

CXX= g++
CXXFLAGS ?= -Wall -std=c++11 -g -I ./include
CPP_SRC=$(wildcard *.cpp)

CC= gcc
CFLAGS=-Wall -g -I ./include
C_SRC= $(wildcard *.c)

LFLAGS= -o

OBJS= $(CPP_SRC:.cpp=.o) $(C_SRC:.c=.o)

%.cpp:%.o
	$(CXX) $(CXXFLAGS) -c $< $@

%.c:%.o
	$(CXX) $(CXXFLAGS) -c $< $@

$(EXECUTABLE): $(OBJS)
	$(CXX) $(OBJS) $(LFLAGS) $@


all: $(EXECUTABLE)

.PHONY: printinfo
printinfo:
	@echo "C++ Sources: " $(CPP_SRC)
	@echo "C Sources: " $(C_SRC)
	@echo "CFLAGS: " $(CFLAGS)
	@echo "CXXFLAGS: " $(CXXFLAGS)
	@echo "OBJS: " $(OBJS)
	@echo "CXX: " $(CPP)
	@echo "CC: " $(CC)

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) *.o
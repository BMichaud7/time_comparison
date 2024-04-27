CC = g++
CFLAGS = -std=c++11
BUILD_SYSTEM ?= make

ifeq ($(BUILD_SYSTEM),ninja)
    BUILD_COMMAND = ninja
    BUILD_FILE = build.ninja
else
    BUILD_COMMAND = make
    BUILD_FILE = Makefile
endif

all: time_comparison

time_comparison: main.cpp
	$(CC) $(CFLAGS) main.cpp -o time_comparison

clean:
	rm -f time_comparison

ninja:
	$(CC) $(CFLAGS) -MMD -MF main.d main.cpp -o main.o
	cmake -G Ninja .
	$(BUILD_COMMAND)

.PHONY: clean ninja


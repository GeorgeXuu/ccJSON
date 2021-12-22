# CFLAGS = -O2
# CFLAGS = -O2 -Wall
# CFLAGS = -g -Wno-write-strings -Wno-unused-variable -Wall

CFLAGS = -g -Wno-write-strings -Wno-unused-variable

WARNINGS = -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual
           
CC = g++
FLAG = -g

#FLAG = -02

all: test

test: test.cpp ccJSON.h
	$(CC) $(CFLAGS) -o test test.cpp ccJSON.cpp -std=c++11 -lm -lc -lpthread $(WARNINGS)

clean:
	rm -rf test

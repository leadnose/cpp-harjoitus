# CC = clang++
# CFLAGS = -g -Wall -Wextra -Werror
CC = g++
CFLAGS = -g -Wall -Wextra -Werror --std=c++0x -DDEBUG


testdriver: testdriver.cc string.hh string.o
	$(CC) $(CFLAGS) -o testdriver testdriver.cc string.o

string.o: string.hh string.cc
	$(CC) $(CFLAGS) -c -o string.o string.cc

testrun: testdriver
	./testdriver 2> test.log

clean:
	rm -f testdriver string.o

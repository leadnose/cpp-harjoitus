# CC = clang++
# CFLAGS = -g -Wall -Wextra -Werror
CC = g++
CFLAGS = -g -Wall -Wextra -Werror --std=c++0x


testdriver: stringtest.cc string.hh string.o
	$(CC) $(CFLAGS) -o testdriver stringtest.cc string.o

string.o: string.hh string.cc
	$(CC) $(CFLAGS) -c -o string.o string.cc

testrun: testdriver
	./testdriver

clean:
	rm -f testdriver string.o
# CC = clang++
# CFLAGS = -g -Wall -Wextra -Werror
CC = g++
CFLAGS = -g -Wall -Wextra --std=c++0x -DDEBUG


testdriver: testdriver.cc string.hh string.o
	$(CC) $(CFLAGS) -o testdriver testdriver.cc string.o

string.o: string.hh string.cc
	$(CC) $(CFLAGS) -c -o string.o string.cc

testrun: testdriver
	./testdriver 2> test.log

leakcheck: testdriver
	valgrind --leak-check=full --show-reachable=yes ./testdriver 2> test.log

clean:
	rm -f testdriver string.o
	rm -rf jronkone/src jronkone/doc
	rmdir jronkone

report: report.tex
	pdflatex report.tex

zip: 
	mkdir -p jronkone/src
	mkdir -p jronkone/doc
	cp *.hh *.cc Makefile jronkone/src
	cp report.pdf jronkone/doc
	zip jronkone jronkone/src/* jronkone/doc/*

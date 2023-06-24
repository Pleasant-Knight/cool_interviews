# Michel Hanna: Makefile for Scala test

CC = g++
CFLAGS = -g -W -Wall
#LIBS = -ldl -lc -lpthread -lyaml-cpp
LIBS = -ldl -lyaml-cpp
OBJS = v1.o generateyaml.o timer.o hikers.o 

all: v1 $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

generateyaml: generateyaml.c hiker.h
	$(CC) $(CFLAGS) -o $@
	
timer: timer.cpp timer.h
	$(CC) $(CFLAGS) -o $@

hikers: hikers.cpp hiker.h 
	$(CC) $(CFLAGS) -o $@

v1: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

#runtime_test: mh_test_memh.c
#	$(CC) $(CFLAGS) -o $@ $^ -L. -lmemh -ldl

.PHONY: clean

clean:
	rm -f *.o v1

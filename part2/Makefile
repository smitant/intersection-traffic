#
# Samantha Foley
# CS 441/541: Project 4 Part 2
#
#####################################################################
#
# Type "make" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g -O0 -I../lib
LDFLAGS=-pthread

PROGS=stoplight

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
all: $(PROGS)

#
# Traffic Management 
#
stoplight: stoplight.c support.o stoplight.h ../lib/semaphore_support.o
	$(CC) -o stoplight stoplight.c support.o ../lib/semaphore_support.o $(CFLAGS) $(LDFLAGS)


#
# Support library
#
support.o: support.c support.h ../lib/semaphore_support.o
	$(CC) -c -o support.o support.c  $(CFLAGS)

#
# Semaphore support library
#
../lib/semaphore_support.o: ../lib/semaphore_support.h ../lib/semaphore_support.c
	cd ../lib && make lib

#
# Cleanup the files that we have created
#
clean:
	$(RM) $(PROGS) *.o
	$(RM) -rf *.dSYM
	cd ../lib && make clean

#
# Josh Hursey
# CS 441/541: Semaphore Library
#
#####################################################################
#
# Type "make" or "make sum" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-pthread -lm

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
PROGS=sum

all: $(PROGS)

#
# Sum program
#
sum: sum.c sum.h semaphore_support.o
	$(CC) -o sum sum.c semaphore_support.o $(CFLAGS) $(LDFLAGS)

#
# Semaphore support library
#
lib: semaphore_support.o

semaphore_support.o: semaphore_support.h semaphore_support.c
	$(CC) -c -o semaphore_support.o semaphore_support.c $(CFLAGS)

#
# Cleanup the files that we have created
#
clean:
	$(RM) $(PROGS) *.o
	$(RM) -rf *.dSYM

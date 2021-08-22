CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic -g
MAIN = smake
OBJS = smake.o smakeFuncs.o HashTable.o readline.o
all : $(MAIN)

$(MAIN) : $(OBJS) smakeFuncs.h HashTable.h readline.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) smakeFuncs.h HashTable.h readline.h

valgrind :
	valgrind --leak-check=full --track-origins=yes ./smake

smake.o : smake.c smakeFuncs.h
	$(CC) $(CFLAGS) -c smake.c

smakeFuncs.o : smakeFuncs.c smakeFuncs.h
	$(CC) $(CFLAGS) -c smakeFuncs.c

HashTable.o : HashTable.c HashTable.h
	$(CC) $(CFLAGS) -c HashTable.c

readline.o : readline.c readline.h
	$(CC) $(CFLAGS) -c readline.c

clean :
	rm *.o $(MAIN) core

testclean :
	rm *.o $(TESTMAIN) core
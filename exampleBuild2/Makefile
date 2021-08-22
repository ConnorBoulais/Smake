CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = mytr
OBJS = mytrFuncs.o mytr.o
all : $(MAIN)

$(MAIN) : $(OBJS) mytrFuncs.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

Valgrind:
	valgrind --leak-check=full --track-origins=yes ./smake > myout

mytr.o : mytr.c mytrFuncs.h
	$(CC) $(CFLAGS) -c mytr.c

mytrFuncs.o : mytrFuncs.c mytrFuncs.h
	$(CC) $(CFLAGS) -c mytrFuncs.c

clean :
	rm *.o $(MAIN) core

CFLAGS	= -I. -Wall -g -std=c99
LFLAGS	= -I. -Wall -g
#LIBS	= -lm
LIBS	=

chalenge: obj/main.o obj/readbuf.o obj/challenge.o
	gcc $(LFLAGS) $(LIBS) obj/main.o obj/readbuf.o obj/challenge.o \
		-o challenge

obj/main.o: main.c challenge.h readbuf.h macrodef.h
	gcc $(CFLAGS) -c main.c -o obj/main.o 

obj/readbuf.o: readbuf.c readbuf.h macrodef.h
	gcc $(CFLAGS) -c readbuf.c -o obj/readbuf.o

obj/challenge.o: challenge.c challenge.h macrodef.h
	gcc $(CFLAGS) -c challenge.c -o obj/challenge.o

clean:
	rm -f obj/*.o challenge

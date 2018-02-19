DIR==/home/atkrye/Documents/EGGP/
INCDIR=/home/atkrye/Documents/EGGP/P-GP2-Include/include
LIBDIR=/home/atkrye/Documents/EGGP/P-GP2-Lib/lib
OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc

CFLAGS = -I$(INCDIR) -L $(LIBDIR) -fomit-frame-pointer -O2 -Wall -lgp2 -Wextra -lm -g

default:	$(OBJECTS)
		$(CC) $(OBJECTS) $(CFLAGS) -o eggp

%.o:		%.c
		$(CC) -c $(CFLAGS) -o $@ $<

clean:
		rm *

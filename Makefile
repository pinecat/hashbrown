# Makefile for compiling libhashbrown
CC=cc
CFLAGS= -Wall -std=c99
ARCHIVE=ar
ELEV=doas
HINSTDIR=/usr/local/include
AINSTDIR=/usr/local/lib

all: libhashbrown.a

libhashbrown.a: hashbrown.o
	$(ARCHIVE) -cvq $@ hashbrown.o

hashbrown.o: hashbrown.c hashbrown.h
	$(CC) $(CFLAGS) -c -o $@ hashbrown.c

install: hashbrown.h libhashbrown.a
	$(ELEV) cp -f hashbrown.h $(HINSTDIR)/
	$(ELEV) cp -f libhashbrown.a $(AINSTDIR)/

uninstall:
	$(ELEV) rm -f $(HINSTDIR)/hashbrown.h
	$(ELEV) rm -f $(AINSTDIR)/libhashbrown.a

clean:
	rm -rf *.o *.a

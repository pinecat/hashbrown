# Makefile for compiling libhashbrown
CC=cc
CFLAGS= -Wall -std=c99
ARCHIVE=ar
BUILDDIR=build
HINSTDIR=/usr/local/include
AINSTDIR=/usr/local/lib

all: libhashbrown.a

libhashbrown.a: hashbrown.o
	$(ARCHIVE) -cvq $(BUILDDIR)/$@ $(BUILDDIR)/hashbrown.o

hashbrown.o: hashbrown.c hashbrown.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/$@ hashbrown.c

install: hashbrown.h libhashbrown.a
	cp -f hashbrown.h $(HINSTDIR)/
	cp -f $(BUILDDIR)/libhashbrown.a $(AINSTDIR)/

uninstall:
	rm -f $(HINSTDIR)/hashbrown.h
	rm -f $(AINSTDIR)/libhashbrown.a

clean:
	rm -rf $(BUILDDIR) *.o *.a

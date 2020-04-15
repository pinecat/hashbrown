# Makefile for compiling libhashbrown
CC=cc
CFLAGS= -Wall -std=c99
ARCHIVE=ar
ELEV=doas
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
	$(ELEV) cp -f hashbrown.h $(HINSTDIR)/
	$(ELEV) cp -f $(BUILDDIR)/libhashbrown.a $(AINSTDIR)/

uninstall:
	$(ELEV) rm -f $(HINSTDIR)/hashbrown.h
	$(ELEV) rm -f $(AINSTDIR)/libhashbrown.a

clean:
	rm -rf $(BUILDDIR) *.o *.a

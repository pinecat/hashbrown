all: hb

hb: hb.c hashbrown.o
	cc -lm -o hb hashbrown.o hb.c

hashbrown.o: hashbrown.c hashbrown.h
	cc -c -o hashbrown.o hashbrown.c

clean:
	rm -rf *.o hb

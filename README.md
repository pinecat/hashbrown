# hashbrown &middot; [![license](https://img.shields.io/badge/license-MIT-blue)](https://github.com/pinecat/hashbrown/blob/master/LICENSE) [![target](https://img.shields.io/badge/OpenBSD-6.6-blue)](https://openbsd.org) [![lang](https://img.shields.io/badge/clang-8.0.1-lightgrey)](https://llvm.org)

A small C library for OpenBSD which implements hash tables using the double-hashing approach to open addressing.

## Open Addressing
You may be wondering, "What exactly is open addressing, and what is the double-hashing approach?".  Open addressing is a technique to handle collision resolution in hash tables.  In open addressing, an algorithm is used to search through the hash table until the target key is found, or until the end of the table, in which case the target key/value pair does not exist.  The approach, double-hashing is the algorithm used to search the table.  In this method, a second hash function is applied to the key that is being searched whenever a collision occurs.

I may not be the best at explaining this, so below you will find some good resources for open addressing and double-hashing in hash tables:
+ [Open Addressing - Wikipedia](https://en.wikipedia.org/wiki/Open_addressing)
+ [Open Addressing - GeeksForGeeks](https://www.geeksforgeeks.org/hashing-set-3-open-addressing/)
+ [Double Hashing - GeeksForGeeks](https://www.geeksforgeeks.org/double-hashing/)
+ [Hashing III: Open Addressing - CSAIL MIT](https://courses.csail.mit.edu/6.006/fall11/lectures/lecture10.pdf)

## Compiling & Installing
Hashbrown has no external dependencies.  To compile and install the code, simply follow the instructions below.

```sh
$ git clone https://github.com/pinecat/hashbrown.git && cd hashbrown
$ make && make install
```

The Makefile will create a directory called build, and place the generated object and archive files there.  The `make install` target will (by default) install the library files in the corresponding directories:

```
hashbrown.h --> /usr/local/include/
libhashbrown.a --> /usr/local/lib/
```

Also, please be aware that the `make install` target will (by default) use the `doas` command to get elevated privileges, **NOT** `sudo`.  You can change this in the Makefile however, by simply setting the `ELEV` variable to `sudo`.

You can clean the directory using `make clean`.  This will remove the build directory, as well as any object files and archive files.

You can uninstall the library using `make uninstall`.  Please be aware, that this also uses the `doas` command (by default) to get elevated privileges.

## Using The Library In Your Own Code
Any code you want to use this library with, you'll have to specify it explicitly in your compiler options.  You will also have to link the library similarly to how you'd link the math library. You can use the `-I` and `-L` options to specify the location of these library files like so:

```
cc -I/usr/local/include -L/usr/local/lib -lhashbrown ...
```

The reason that the include and library directories must be explicitly specified, is because the install locations are not part of the default library search path.  You can find more info about the default library search path for `cc` [here](https://nanxiao.me/en/the-header-file-and-library-search-path-of-cc-on-openbsd/).  Of course, you can change the install directories in the Makefile so that the library does show up in the default library path -- if you so desire.

Below is some example code, with output.  There is also an example Makefile.

### Example Code
```c
// File: mymap.c

#include <stdio.h>
#include <hashbrown.h>

int main() {
    // Create the map
    hb_map *map = hb_newmap();

    // Put something in the map
    hb_put(map, "hello", "world");

    // See if a key is found in the map
    printf("Is the key \"hello\" in the map? %s\n", hb_haskey(map, "hello") ? "Yes" : "No");
    printf("Is the key \"foo\" in the map? %s\n", hb_haskey(map, "foo") ? "Yes" : "No");

    // Try to get a value from the map using a key (successful)
    char val1[512];
    int success1 = hb_get(map, "hello", val1);
    printf("[%d] The key \"hello\" returned the value: %s\n", success1, val1);

    // Try to get a value from the map using a key (unsuccessful)
    char val2[512];
    int success2 = hb_get(map, "foo", val2);
    printf("[%d] The key \"foo\" returned the value: %s\n", success2, val2);

    // Free the map
    hb_freemap(map);

    // Exit
    return 0;
}
```

```makefile
# File: Makefile

CC=cc
IDIR=/usr/local/include
LDIR=/usr/local/lib
CFLAGS= -Wall -I$(IDIR) -L$(LDIR) -std=c99
LIBS= -lhashbrown

all: mymap

mymap: main.c
    $(CC) $(CFLAGS) $(LIBS) -o $@ mymap.c

clean:
    rm -rf *.o mymap
```

```sh
# Output of mymap

$ ./mymap
Is the key "hello" in the map? Yes
Is the key "foo" in the map? No
[1] The key "hello" returned the value: world
[0] The key "foo" returned the value:
```

## Licensing
Licensed under the [MIT license](https://github.com/pinecat/hashbrown/blob/master/LICENSE).
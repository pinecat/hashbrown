/*
 * hashbrown
 * Implementation of hash tables in C using the double-hashing approach to open addressing.
 * 
 * Created by Rory (aka pinecat).
 * Licensed under the MIT license.
 */

#include "hashbrown.h"

/*
    hb_newmap()
        Initiliazes a map pointer and initializes
        the table of the map.
    Params:
        n/a
    Returns:
        hb_map * - map - A pointer to a new hashmap.
*/
hb_map *hb_newmap() {
    hb_map *map = (hb_map *)malloc(sizeof(hb_map));
    map->table = (hb_node *)malloc(sizeof(hb_node) * 101);
    map->len = 101;
    map->size = 0;
    return map;
}

/*
    hb_pow()
        Helper subroutine to compute the value of
        x to the exponent n (x^n <-- not XOR lol).
    Params:
        int - x - The base.
        int - n - The exponent.
    Returns:
        int - - x to the exponent n.
*/
int hb_pow(int x, int n) {
    if (n == 0) {
        return 1;
    } else if (n % 2 == 0) {
        return (hb_pow(x, n / 2) * hb_pow(x, n / 2));
    } else {
        return (x * hb_pow(x, n / 2) * hb_pow(x, n / 2));
    }
}

/*
    hb_hashcode()
        Subroutine which computes the hashcode of
        a given string.
    Params:
        char * - str - The string to compute the hashcode of.
    Returns:
        int - hashcode - The computed hashcode of the given string.
*/
int hb_hashcode(char *str) {
    int hashcode = 0;
    int len = strlen(str);
    int i = 0;
    while (str[i] != 0) {
        hashcode += (str[i] * hb_pow(31, len - (i + 1)));
        i++;
    }
    return hashcode;
}

/*
    hb_hash()
        Used to compute the hash of a key in the table,
        or of a new key.
    Params:
        int - len - The length of the hash map's table.
        char * - key - The key to compute the hash for.
    Returns:
        int - - The hash of the given key.
*/
int hb_hash(int len, char *key) {
    return ((hb_hashcode(key) & 0x7fffffff) % len);
}

/*
    hb_step()
        Used to calculate the step size when searching
        for an unused bucket.
    Params:
        char * - key - The key to compute the step for.
    Returns:
        int - - The step for the given key.
*/
int hb_step(char *key) {
    return (((hb_hashcode(key) & 0x7fffffff) % 97) + 1);
}

/*
    hb_nextprime()
        Subroutine which takes the current table length of a map,
        multiplies it by 2, then finds the next prime number
        larger than that value.
    Params:
        int - len - The length of the hash map's table.
    Returns:
        int - new_len - The next prime number larger than
                        (the prev length * 2)
*/
int hb_nextprime(int len) {
    int new_len = len * 2;
    while (!hb_isprime(new_len++));
    return new_len;
}

/*
    hb_isprime()
        Subroutine to determine whether or not
        a given number is prime.
    Params:
        int - n - The number to test for primeness.
    Returns:
        int - 0 (false) - If the number is not prime.
        int - 1 (true) - If the number is prime.
*/
int hb_isprime(int n) {
    int i;
    for (i = 2; (i * i) <= n; i++) {
        if (n % i == 0) { return 0; }
    }
    return 1;
}

/*
    hb_haskey()
        Determines whether or not a given key
        exists in a given map.
    Params:
        hb_map * - map - The map to lookup entries in.
        char * - key - The key to search for in the map.
    Returns:
        int - 0 (false) - If the key is not found.
        int - 1 (true) - If the key is found.
*/
int hb_haskey(hb_map *map, char *key) {
    int hash = hb_hash(map->len, key);
    int step = hb_step(key);
    hb_node temp = map->table[hash];

    while (temp.key[0] != 0) {
        if (!strcmp(temp.key, key)) {
            return 1;
        } else {
            hash += step;
            if (hash >= map->len) {
                hash -= map->len;
            }
            temp = map->table[hash];
        }
    }
    return 0;
}

/*
    hb_put()
        Puts a given key/value pair in a given map.
        Or, if the key already exists, will update
        the value of the specified key.
    Params:
        hb_map * - map - The map to add/update the key/value pair in.
        char * - key - The key to create the entry with.
        char * - val - The value to create the entry with.
    Returns:
        int - 0 - If a key/value pair was updated.
        int - 1 - If a key/value pair was added.
*/
int hb_put(hb_map *map, char *key, char *val) {
    double load = map->size / (double) map->len;
    if (load > 0.75) {
        int new_len = hb_nextprime(map->len * 2);
        map->table = (hb_node *)realloc(map->table, sizeof(hb_node) * new_len);
        map->len = new_len;
    }

    int hash = hb_hash(map->len, key);
    int step = hb_step(key);
    hb_node temp = map->table[hash];

    while (temp.key[0] != 0) {
        if (!strcmp(temp.key, key)) {
            strcpy(temp.val, val);
            return 0;
        } else {
            hash += step;
            if (hash >= map->len) {
                hash -= map->len;
            }
            temp = map->table[hash];
        }
    }

    hb_node node;
    strcpy(node.key, key);
    strcpy(node.val, val);
    map->table[hash] = node;
    map->size++;
    return 1;
}

/*
    hb_get()
        Retrieves a value from the given map
        using the given key.
        The val paramater will be used to store
        the result of the table entry, if said
        table entry is found with the given key.
        So, an empty buffer should be passed in
        for this parameter.
    Params:
        hb_map * - map - The map to retrieve the entry from.
        char * - key - The key used to retrieve the value.
        char * - val - A buffer which will store the value
                        (if found) from the hash table entry.
    Returns:
        int - 0 - If a key/value entry was not found.
        int - 1 - If a key/value entry was found.
*/
int hb_get(hb_map *map, char *key, char *val) {
    int hash = hb_hash(map->len, key);
    int step = hb_step(key);
    hb_node temp = map->table[hash];
    while (temp.key[0] != 0) {
        if (!strcmp(temp.key, key)) {
            strcpy(val, temp.val);
            return 1;
        } else {
            hash += step;
            if (hash >= map->len) {
                hash -= map->len;
            }
            temp = map->table[hash];
        }
    }
    return 0;
}

/*
    hb_freemap()
        Frees all memory in use by the map,
        including the table of the map.
    Params:
        hb_map * - map - The map to be freed.
    Returns:
        void
*/
void hb_freemap(hb_map *map) {
    free(map->table);
    free(map);
}
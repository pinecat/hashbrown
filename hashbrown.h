#ifndef HASHBROWN_H
#define HASHBROWN_H

/* imports */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* constants */
#define MAX_STR_LEN 256

/* structs */
typedef struct hb_node {
    char key[MAX_STR_LEN];
    char val[MAX_STR_LEN];
} hb_node;

typedef struct hb_map {
    hb_node *table;
    int     len;
    int     size;
} hb_map;

/* prototypes */
hb_map *hb_newmap();
int hb_hashcode(char *str);
int hb_hash(int len, char *key);
int hb_step(char *key);
int hb_nextprime(int len);
int hb_isprime(int n);
int hb_haskey(hb_map *map, char *key);
int hb_put(hb_map *map, char *key, char *val);
int hb_get(hb_map *map, char *key, char *val);
void hb_freemap(hb_map *map);

#endif
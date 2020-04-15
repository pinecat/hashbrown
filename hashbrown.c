#include "hashbrown.h"

hb_map *hb_newmap() {
    hb_map *map = (hb_map *)malloc(sizeof(hb_map));
    map->table = (hb_node *)malloc(sizeof(hb_node) * 101);
    map->len = 101;
    map->size = 0;
    return map;
}

int hb_pow(int x, int n) {
    if (n == 0) {
        return 1;
    } else if (n % 2 == 0) {
        return (hb_pow(x, n / 2) * hb_pow(x, n / 2));
    } else {
        return (x * hb_pow(x, n / 2) * hb_pow(x, n / 2));
    }
}

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

int hb_hash(int len, char *key) {
    return ((hb_hashcode(key) & 0x7fffffff) % len);
}

int hb_step(char *key) {
    return (((hb_hashcode(key) & 0x7fffffff) % 97) + 1);
}

int hb_nextprime(int len) {
    int new_len = len * 2;
    while (!hb_isprime(new_len++));
    return new_len;
}

int hb_isprime(int n) {
    int i;
    for (i = 2; (i * i) <= n; i++) {
        if (n % i == 0) { return 0; }
    }
    return 1;
}

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

void hb_freemap(hb_map *map) {
    free(map->table);
    free(map);
}
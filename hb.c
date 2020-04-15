#include <stdio.h>
#include "hashbrown.h"

int main() {
    hb_map *map = hb_newmap();
    hb_put(map, "hello", "world");
    printf("Is in map: %d\n", hb_haskey(map, "hello"));
    printf("Is in map: %d\n", hb_haskey(map, "whatevs"));
    char val[512];
    int suc = hb_get(map, "hello", val);
    printf("%d: Val of \"hello\": %s\n", suc, val);
    char val2[512];
    int suc2 = hb_get(map, "whateves", val2);
    printf("%d: Val of \"whatevs\": %s\n", suc2, val2);
    hb_freemap(map);
    return 0;
}
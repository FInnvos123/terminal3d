#include <stdlib.h>
#include <stdio.h>

#include "map.h"

char * read_map(char *filename, int width, int height)
{
    char *map = malloc(sizeof(char) * width * height);

    FILE *fp = fopen(filename, "r");

    int x = 0, y = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            x = 0;
            y++;
        } else {
            map[x++ + y * width] = c;
        }
    }

    fclose(fp);

    return map;
}

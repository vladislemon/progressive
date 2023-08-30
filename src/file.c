#include "file.h"
#include <stdio.h>
#include <stdlib.h>

char *loadFile(const char *path, usize *size_out) {
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(size);
    fread(data, 1, size, file);
    fclose(file);
    *size_out = size;
    return data;
}

#ifndef PROGRESSIVE_TEXTURE_H
#define PROGRESSIVE_TEXTURE_H

#include "types.h"

typedef struct Texture_s {
    u32 id;
    u32 width;
    u32 height;
} Texture;

Texture texture_create(const char *fileName);

void texture_bind(Texture *texture);

void texture_unbind();

#endif //PROGRESSIVE_TEXTURE_H

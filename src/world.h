#ifndef PROGRESSIVE_WORLD_H
#define PROGRESSIVE_WORLD_H

#include "types.h"

typedef u8* World;

World world_create();

u8 world_getBlock(World world, i32 x, i32 y, i32 z);

void world_setBlock(World world, i32 x, i32 y, i32 z, u8 block);

void world_fill(World world, u8 block);

void world_fillRegion(World world, i32 minX, i32 minY, i32 minZ, i32 maxX, i32 maxY, i32 maxZ, u8 block);

void world_iterate(World world, void (*f)(World world, i32 x, i32 y, i32 z, u8 block, void *custom), void *custom);

#endif //PROGRESSIVE_WORLD_H

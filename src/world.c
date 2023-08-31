#include "world.h"
#include <stdlib.h>
#include <string.h>

#define SIZE 256
#define VOLUME (SIZE * SIZE * SIZE)
#define OFFSET(x, y, z) (y * SIZE * SIZE + z * SIZE + x)

World world_create() {
    return malloc(VOLUME + 1);
}

u8 getBlock(World world, i32 x, i32 y, i32 z) {
    return world[OFFSET(x, y, z)];
}

void world_setBlock(World world, i32 x, i32 y, i32 z, u8 block) {
    world[OFFSET(x, y, z)] = block;
    world[VOLUME] = true;
}

void world_fill(World world, u8 block) {
    memset(world, block, VOLUME);
}

void world_fillRegion(World world, i32 minX, i32 minY, i32 minZ, i32 maxX, i32 maxY, i32 maxZ, u8 block) {
    for (int y = minY; y <= maxY; ++y) {
        for (int z = minZ; z <= maxZ; ++z) {
            for (int x = minX; x <= maxX; ++x) {
                world_setBlock(world, x, y, z, block);
            }
        }
    }
}

void world_iterate(World world, void (*f)(World world, i32 x, i32 y, i32 z, u8 block, void *custom), void *custom) {
    for (int i = 0; i < VOLUME; i++) {
        i32 x = i % SIZE;
        i32 y = (i / SIZE) / SIZE;
        i32 z = (i / SIZE) % SIZE;
        f(world, x, y, z, world[i], custom);
    }
}

bool world_is_updated(World world, bool clear) {
    bool isUpdated = world[VOLUME];
    if (clear) {
        world[VOLUME] = false;
    }
    return isUpdated;
}

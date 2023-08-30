#include "mesh_builder.h"
#include <stdlib.h>
#include <string.h>

#define MESH_UNIT_SIZE 5
#define INVERT_V_COORD

struct MeshContainer {
    float *mesh;
    usize *meshSize;
};

static float CUBE_VERTICES[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f
};

static float CUBE_MESH[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f,

        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f
};

static const float CUBE_UV[] = {
        0.000059f, 1.0f - 0.000004f,
        0.000103f, 1.0f - 0.336048f,
        0.335973f, 1.0f - 0.335903f,
        1.000023f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.336024f, 1.0f - 0.671877f,
        0.667969f, 1.0f - 0.671889f,
        1.000023f, 1.0f - 0.000013f,
        0.668104f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.000059f, 1.0f - 0.000004f,
        0.335973f, 1.0f - 0.335903f,
        0.336098f, 1.0f - 0.000071f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.336024f, 1.0f - 0.671877f,
        1.000004f, 1.0f - 0.671847f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.668104f, 1.0f - 0.000013f,
        0.335973f, 1.0f - 0.335903f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.668104f, 1.0f - 0.000013f,
        0.336098f, 1.0f - 0.000071f,
        0.000103f, 1.0f - 0.336048f,
        0.000004f, 1.0f - 0.671870f,
        0.336024f, 1.0f - 0.671877f,
        0.000103f, 1.0f - 0.336048f,
        0.336024f, 1.0f - 0.671877f,
        0.335973f, 1.0f - 0.335903f,
        0.667969f, 1.0f - 0.671889f,
        1.000004f, 1.0f - 0.671847f,
        0.667979f, 1.0f - 0.335851f
};

static void addBlockMesh(float *mesh, usize *meshSize, i32 x, i32 y, i32 z) {
    int cubeMeshSize = sizeof(CUBE_MESH) / sizeof(float);
    float *start = mesh + *meshSize;
    memcpy(start, CUBE_MESH, sizeof(CUBE_MESH));
    for (int i = 0; i < cubeMeshSize; i += MESH_UNIT_SIZE) {
        start[i] += (float) x;
        start[i + 1] += (float) y;
        start[i + 2] += (float) z;
#ifdef INVERT_V_COORD
        start[i + 4] = 1.0f - start[i + 4];
#endif
    }
    *meshSize += cubeMeshSize;
}

static void addBlockMeshIteration(World world, i32 x, i32 y, i32 z, u8 block, void *custom) {
    if (block == 0) {
        return;
    }
    struct MeshContainer container = *(struct MeshContainer *) custom;
    addBlockMesh(container.mesh, container.meshSize, x, y, z);
}

void mesh_builder_build(World world, float **mesh, usize *meshSize) {
    *mesh = malloc(16 * 16 * 16 * sizeof(CUBE_MESH));
    *meshSize = 0;
    struct MeshContainer container = {*mesh, meshSize};
    world_iterate(world, addBlockMeshIteration, &container);
}

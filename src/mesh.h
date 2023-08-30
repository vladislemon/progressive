#ifndef PROGRESSIVE_MESH_H
#define PROGRESSIVE_MESH_H

#include "types.h"

typedef struct Mesh_s {
    u32 vao;
    u32 vbo;
    u32 size;
} Mesh;

Mesh mesh_create();

void mesh_update(Mesh *mesh, isize size, float *vertices);

void mesh_bind(Mesh *mesh);

void mesh_draw(Mesh *mesh);

#endif //PROGRESSIVE_MESH_H

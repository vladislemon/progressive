#include "mesh.h"
#include <glad/gl.h>

Mesh mesh_create() {
    Mesh mesh;
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) (3 * sizeof(float)));
    return mesh;
}

static void update_data(u32 vbo, isize size, float *data) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void mesh_update(Mesh *mesh, isize size, float *vertices) {
    update_data(mesh->vbo, size * (isize) sizeof(float), vertices);
    mesh->size = size;
}

void mesh_bind(Mesh *mesh) {
    glBindVertexArray(mesh->vao);
}

void mesh_draw(Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, (GLint) mesh->size);
}

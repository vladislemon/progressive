#include "texture.h"
#include "lodepng.h"
#include <stdlib.h>
#include <glad/gl.h>

Texture texture_create(const char *fileName) {
    Texture texture;
    u8 *data;
    lodepng_decode32_file(&data, &texture.width, &texture.height, fileName);
    glGenTextures(1, &texture.id);
    texture_bind(&texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 (i32) texture.width, (i32) texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(data);
    texture_unbind();
    return texture;
}

void texture_bind(Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

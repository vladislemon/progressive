#ifndef PROGRESSIVE_SHADER_PROGRAM_H
#define PROGRESSIVE_SHADER_PROGRAM_H

#include <glad/gl.h>

typedef GLint ShaderProgram;

ShaderProgram shader_program_create(const char *vertexPath, const char *fragmentPath);

int shader_program_get_uniform_location();

void shader_program_use(ShaderProgram shaderProgram);

#endif //PROGRESSIVE_SHADER_PROGRAM_H

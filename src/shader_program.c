#include "shader_program.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

ShaderProgram shader_program_create(const char *vertexPath, const char *fragmentPath) {
    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    usize vertexSourceSize;
    usize fragmentSourceSize;
    char *vertexSource = loadFile(vertexPath, &vertexSourceSize);
    char *fragmentSource = loadFile(fragmentPath, &fragmentSourceSize);
    glShaderSource(vertexShader, 1, (const GLchar *const *) &vertexSource, (const GLint *) &vertexSourceSize);
    glShaderSource(fragmentShader, 1, (const GLchar *const *) &fragmentSource, (const GLint *) &fragmentSourceSize);
    int success;
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertexShader, sizeof(log), 0, log);
        printf("%s\n", log);
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, sizeof(log), 0, log);
        printf("%s\n", log);
    }
    ShaderProgram shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, sizeof(log), 0, log);
        printf("%s\n", log);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);
    return shaderProgram;
}

void shader_program_use(ShaderProgram shaderProgram) {
    glUseProgram(shaderProgram);
}

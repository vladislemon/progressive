#version 450 core

layout (location = 0) uniform mat4 mvp;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 textureCoords_frag;

void main() {
    gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);
    textureCoords_frag = textureCoords;
}
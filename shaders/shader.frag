#version 450 core

uniform sampler2D texture0;

in vec2 textureCoords_frag;

out vec4 FragColor;

void main() {
    FragColor = texture(texture0, textureCoords_frag);
}
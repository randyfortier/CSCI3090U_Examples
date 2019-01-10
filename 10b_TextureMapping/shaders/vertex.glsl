#version 110

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;

uniform mat4 MVP;

void main() {
    gl_Position =  MVP * vec4(position, 1.0);
}

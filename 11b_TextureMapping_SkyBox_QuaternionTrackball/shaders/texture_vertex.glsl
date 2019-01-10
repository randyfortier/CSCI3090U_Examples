#version 110

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec4 u_DiffuseColour;

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;

varying vec2 v_textureCoords;

void main() {
    gl_Position =  u_MVPMatrix * vec4(position, 1.0);

    v_textureCoords = textureCoords;
}

#version 110

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;

uniform sampler2D textureSampler;
uniform mat4 MVP;

varying vec2 v_TextureCoords;
varying vec3 v_Normal;

void main() {
   // interpolated parameters
   v_TextureCoords = textureCoords;
   v_Normal = normal;  // TODO: transform into proper coordinate sys

   // MVP transform to find position
   gl_Position =  MVP * vec4(position, 1.0);
}

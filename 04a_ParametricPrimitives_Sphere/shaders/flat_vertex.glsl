uniform mat4 u_MVP;

attribute vec4 position;
attribute vec2 textureCoords;
attribute vec3 normal;

void main() {
    gl_Position = u_MVP * position;
}

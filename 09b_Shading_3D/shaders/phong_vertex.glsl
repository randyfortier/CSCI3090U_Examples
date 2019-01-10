uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec4 u_DiffuseColour;

attribute vec4 position;
attribute vec3 normal;

void main() {
    gl_Position = u_MVPMatrix * position;
}

uniform vec3 u_EyePosition;
uniform mat4 u_ModelMatrix;
uniform mat4 u_MVPMatrix;

attribute vec3 position;
attribute vec3 normal;

void main() {
  gl_Position = u_MVPMatrix * vec4(position, 1.0);
}

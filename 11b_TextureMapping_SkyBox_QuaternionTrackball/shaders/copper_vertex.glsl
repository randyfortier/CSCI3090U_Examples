uniform mat4 u_ModelMatrix;
uniform mat4 u_MVPMatrix;

attribute vec3 position;
attribute vec3 normal;

varying vec3 v_Normal;
varying vec3 v_Position;

void main() {
   v_Normal = mat3(u_ModelMatrix) * normal;

   v_Position = vec3(u_ModelMatrix * vec4(position, 1.0));

   gl_Position = u_MVPMatrix * vec4(position, 1.0);
}

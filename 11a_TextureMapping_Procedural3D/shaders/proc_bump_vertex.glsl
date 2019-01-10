attribute vec4 position;
attribute vec3 normal;
attribute vec2 textureCoords;

uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;

varying vec3 v_Normal;
varying vec3 v_Position;
varying vec3 v_Position_Model;

void main() {
   v_Position = vec3(u_MVMatrix * position);
   v_Position_Model = position;
   v_Normal = vec3(u_MVMatrix * vec4(normal, 0.0));

   gl_Position = u_MVPMatrix * position;
}

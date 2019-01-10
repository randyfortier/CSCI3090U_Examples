attribute vec4 position;
attribute vec3 normal;
attribute vec2 textureCoords;

uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;

varying vec3 v_Normal;
varying vec2 v_TexCoord2D;
varying vec3 v_TexCoord3D;
varying vec3 v_Position;

void main() {
   v_Normal = vec3(u_MVMatrix * vec4(normal, 0.0));
   v_Position = vec3(u_MVMatrix * position);

   v_TexCoord2D = textureCoords;
   v_TexCoord3D = position;

   gl_Position = u_MVPMatrix * position;
}

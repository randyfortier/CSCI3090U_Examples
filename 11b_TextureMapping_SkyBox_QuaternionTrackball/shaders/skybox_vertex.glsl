attribute vec3 position;

uniform mat4 u_MVMatrix;
uniform mat4 u_PMatrix;

void main() {
   mat4 rotationMatrix = mat4(mat3(u_MVMatrix));

   gl_Position = u_PMatrix * rotationMatrix * vec4(position, 1.0);
}

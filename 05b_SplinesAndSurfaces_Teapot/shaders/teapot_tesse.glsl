uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * position;
}

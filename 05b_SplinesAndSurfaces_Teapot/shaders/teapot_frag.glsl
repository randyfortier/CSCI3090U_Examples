#version 410 core

in TES_OUT {
  vec3 N;
} fs_in;

void main() {
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(vec3(1.0, 1.0, 1.0));
  vec4 baseColour = vec4(1.0, 0.0, 0.0, 1.0);

  gl_FragColor = 0.5 * baseColour + 0.7 * dot(N, L) * baseColour;
}

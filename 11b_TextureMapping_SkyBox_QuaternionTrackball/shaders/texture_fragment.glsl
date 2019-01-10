#version 130

uniform sampler2D u_TextureSampler;

varying vec2 v_textureCoords;

void main() {
  gl_FragColor = vec4(texture(u_TextureSampler, v_textureCoords).rgb, 1.0);
}

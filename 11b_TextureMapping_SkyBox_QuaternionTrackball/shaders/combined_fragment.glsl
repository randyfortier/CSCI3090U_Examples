#version 130

uniform vec3 u_EyePosition;
uniform vec3 u_LightPos;
uniform samplerCube u_TextureSampler;
uniform float u_Shininess;

void main() {
   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

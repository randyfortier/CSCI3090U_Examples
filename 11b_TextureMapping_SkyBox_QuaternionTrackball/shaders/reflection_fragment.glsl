uniform vec3 u_EyePosition;
uniform samplerCube u_TextureSampler;

varying vec3 v_Normal;
varying vec3 v_Position;

void main() {
   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

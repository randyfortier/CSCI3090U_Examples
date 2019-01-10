uniform vec3 u_EyePosition;
uniform samplerCube u_TextureSampler;

varying vec3 v_Normal;
varying vec3 v_Position;

void main() {
   vec4 baseColour = vec4(0.8, 0.0, 0.0, 1.0);

   vec3 N = normalize(v_Normal);
   vec3 I = normalize(v_Position - u_EyePosition);
   vec3 R = reflect(I, N);

   vec4 textureColour = vec4(texture(u_TextureSampler, R).rgb, 1.0);

   gl_FragColor = mix(baseColour, textureColour, 0.8);
}

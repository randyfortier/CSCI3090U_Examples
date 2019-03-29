uniform samplerCube u_TextureSampler;

varying vec3 v_TexCoords;

void main() {
   gl_FragColor = texture(u_TextureSampler, v_TexCoords);
}

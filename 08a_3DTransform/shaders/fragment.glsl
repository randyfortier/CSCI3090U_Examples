varying vec3 vNormal;

void main() {
   vec3 N = normalize(vNormal);
   vec3 L = normalize(vec3(1.0, 1.0, 1.0));
   vec4 baseColour = vec4(1.0, 0.0, 0.0, 1.0);

   gl_FragColor = 0.5 * baseColour + 0.7 * dot(N, L) * baseColour;
}

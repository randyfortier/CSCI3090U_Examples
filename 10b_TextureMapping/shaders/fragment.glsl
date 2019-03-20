#version 130

uniform sampler2D textureSampler;

varying vec2 v_TextureCoords;
varying vec3 v_Normal;

void main() {
   vec4 baseColour = vec4(texture(textureSampler, v_TextureCoords).rgb, 1.0);

   gl_FragColor = baseColour;
}

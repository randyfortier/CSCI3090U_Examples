uniform mat4 uMVP;
uniform mat4 uMV;

attribute vec4 position;
attribute vec2 textureCoords;
attribute vec3 normal;

varying vec3 vNormal;

void main() {
    gl_Position = uMVP * position;

    vNormal = vec3(uMV * vec4(normal, 0.0));
}

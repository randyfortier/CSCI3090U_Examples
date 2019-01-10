uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec3 u_LightPos;
uniform vec4 u_DiffuseColour;

attribute vec4 position;
attribute vec3 normal;

varying vec4 v_Colour;

void main() {
    vec4 ambientColour = vec4(0.1, 0.1, 0.1, 1.0) * u_DiffuseColour;

    // Transform the vertex into eye space.
    vec3 position_worldspace = vec3(u_MVMatrix * position);

    // Transform the normal's orientation into eye space.
    vec3 normal_worldspace = normalize(vec3(u_MVMatrix * vec4(normal, 0.0)));

    // Will be used for attenuation.
    float distance = length(u_LightPos - position_worldspace);

    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(u_LightPos - position_worldspace);

    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse = clamp(dot(normal_worldspace, lightVector), 0, 1);

    // Attenuate the light based on distance.
    diffuse = diffuse * (1.0 / (1.0 + (0.00025 * distance * distance)));

    // Multiply the color by the illumination level. It will be interpolated across the triangle.
    v_Colour = u_DiffuseColour * diffuse + ambientColour;

    // gl_Position is a special variable used to store the final position.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = u_MVPMatrix * position;
}

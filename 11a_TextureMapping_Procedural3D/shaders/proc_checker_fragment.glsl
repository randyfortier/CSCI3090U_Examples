uniform vec3 u_LightPos;
uniform vec4 u_DiffuseColour;
uniform vec3 u_EyePosition;
uniform float u_Shininess;

varying vec3 v_Normal;
varying vec2 v_TexCoord;
varying vec3 v_Position;

void main() {
   // procedurally-generated texture

   // TODO:  Determine the base colour, based on the texture coordinates

   // lighting and shading
   vec3 normal = normalize(v_Normal);

   // ambient
   vec4 ambientColour = vec4(0.1, 0.1, 0.1, 1.0);

   // diffuse
   float distance = length(u_LightPos - v_Position);
   vec3 lightVector = normalize(u_LightPos - v_Position);
   float diffuse = clamp(dot(normal, lightVector), 0, 1);
   diffuse = diffuse * (1.0 / (1.0 + (0.00025 * distance * distance)));

   // specular
   vec3 incidence = -lightVector;
   vec3 reflection = reflect(incidence, normal);
   vec3 eyeVector = normalize(u_EyePosition - v_Position);
   float cosAngle = max(0.0, dot(eyeVector, reflection));
   float specular = pow(cosAngle, u_Shininess);

   gl_FragColor = specular * baseColour + diffuse * baseColour + ambientColour;
}

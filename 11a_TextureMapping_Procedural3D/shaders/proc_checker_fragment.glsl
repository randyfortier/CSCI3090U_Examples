uniform vec3 u_LightPos;
uniform vec4 u_DiffuseColour;
uniform vec3 u_EyePosition;
uniform float u_Shininess;

varying vec3 v_Normal;
varying vec2 v_TexCoord;
varying vec3 v_Position;

void main() {
   // procedurally-generated texture

   // Determine the base colour, based on the texture coordinates
   int stride = 4;
   int size = 20;
   vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
   vec4 gray = vec4(0.25, 0.25, 0.25, 1.0);

   vec4 baseColour;
   int row, col;

   col = floor(v_TexCoord.s * size);
   row = floor(v_TexCoord.t * size);

   int sum = row / stride + col / stride;
   if (mod(sum, 2) == 0) {
      baseColour = white;
   } else {
      baseColour = gray;
   }

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
   vec4 specularColour = vec4(1.0, 1.0, 1.0, 1.0);
   vec3 incidence = -lightVector;
   vec3 reflection = reflect(incidence, normal);
   vec3 eyeVector = normalize(u_EyePosition - v_Position);
   float cosAngle = max(0.0, dot(eyeVector, reflection));
   float specular = pow(cosAngle, u_Shininess);

   gl_FragColor = specular * specularColour +
                  diffuse * baseColour +
                  ambientColour;
}

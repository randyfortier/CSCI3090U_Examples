uniform vec3 u_LightPos;
uniform vec4 u_DiffuseColour;
uniform vec3 u_EyePosition;
uniform float u_Shininess;

varying vec3 v_Normal;
varying vec3 v_Position;
varying vec3 v_Position_Model;

float bump(float heightScale, float widthScale, float coord) {
   return 1.0;
}

void main() {
   vec4 baseColour = u_DiffuseColour;

   // procedurally-modify the normals
   float bumpHeightScale = 10;
   float bumpWidthScale = 300;

   float x = v_Position_Model.x;
   float y = v_Position_Model.y;
   float z = v_Position_Model.z;

   vec3 normal = v_Normal;

   normal.x += bumpHeightScale * sin(bumpWidthScale * x);
   normal.y += bumpHeightScale * sin(bumpWidthScale * y);
   normal.z += bumpHeightScale * sin(bumpWidthScale * z);

   normal = normalize(normal);

   // lighting and shading

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

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec3 u_LightPos; // view coords
uniform vec4 u_DiffuseColour;

attribute vec4 position;
attribute vec3 normal;

varying vec4 v_Colour;

void main() {
   vec3 position_viewspace = vec3(u_MVMatrix * position);
   vec3 normal_viewspace = normalize(vec3(u_MVMatrix * vec4(normal, 0.0)));

   float distance = length(u_LightPos - position_viewspace);

   vec3 lightVector_viewspace = normalize(u_LightPos - position_viewspace);

   // ambient
   vec4 ambientColour = vec4(0.1, 0.1, 0.1, 1.0) * u_DiffuseColour;

   // diffuse
   float diffuse = clamp(dot(normal_viewspace, lightVector_viewspace), 0, 1);

   // TODO:  Attenuate


   // TODO: specular

   v_Colour = u_DiffuseColour * diffuse + ambientColour;
   gl_Position = u_MVPMatrix * position;
}

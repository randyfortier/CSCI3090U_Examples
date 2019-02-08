#version 410 core

layout (quads, equal_spacing, cw) in;

uniform mat4 u_MVP;

out TES_OUT {
   vec3 N;
} tes_out;

vec4 quadratic_bezier(vec4 a, vec4 b, vec4 c, float t) {
   vec4 d = mix(a, b, t);
   vec4 e = mix(b, c, t);

   return mix(d, e, t);
}

vec4 cubic_bezier(vec4 a, vec4 b, vec4 c, vec4 d, float t) {
   vec4 e = mix(a, b, t);
   vec4 f = mix(b, c, t);
   vec4 g = mix(c, d, t);

   return quadratic_bezier(e, f, g, t);
}

vec4 evaluate_patch(vec2 at) {
   vec4 P[4];
   int i;

   for (i = 0; i < 4; i++) {
      P[i] = cubic_bezier(gl_in[i + 0].gl_Position,
                          gl_in[i + 4].gl_Position,
                          gl_in[i + 8].gl_Position,
                          gl_in[i + 12].gl_Position,
                          at.y);
   }

   return cubic_bezier(P[0], P[1], P[2], P[3], at.x);
}

const float epsilon = 0.001;

void main() {
   // find our point on the patch
   vec4 p1 = evaluate_patch(gl_TessCoord.xy);

   // find a few more points on the patch
   vec4 p2 = evaluate_patch(gl_TessCoord.xy + vec2(0.0, epsilon));
   vec4 p3 = evaluate_patch(gl_TessCoord.xy + vec2(epsilon, 0.0));

   vec3 v1 = normalize(p2.xyz - p1.xyz);  // y-axis
   vec3 v2 = normalize(p3.xyz - p1.xyz);  // x-axis

   // calculate a surface normal at p1 (cross product)
   tes_out.N = cross(v1, v2);

   gl_Position = u_MVP * p1;
}

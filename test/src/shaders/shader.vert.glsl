#version 450

layout(location = 0) in vec3 a_pos;

layout(location = 0) out vec3 v_colour;

layout (binding = 0) uniform ModelViewMatrix {
  mat4 matrix;
} u_mv;

layout (binding = 1) uniform ProjectionMatrix {
  mat4 matrix;
} u_p;

void main() {
  gl_Position = vec4(a_pos, 1);
  v_colour = a_pos + 0.5;
}

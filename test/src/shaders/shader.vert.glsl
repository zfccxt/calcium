#version 450

layout(location = 0) in vec3 a_pos;

layout(location = 0) out vec3 v_colour;

layout (binding = 0) uniform ModelMatrix {
  mat4 matrix;
} u_model;

layout (binding = 1) uniform ViewProjectionMatrix {
  mat4 matrix;
} u_viewprojection;

void main() {
  gl_Position = u_viewprojection.matrix * u_model.matrix * vec4(a_pos, 1);
  v_colour = a_pos + 0.5;
}

#version 450

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;

layout(location = 0) out vec3 v_colour;
layout(location = 1) out vec2 v_texcoord;

layout (binding = 0) uniform ModelMatrix {
  mat4 matrix;
} u_model;

layout (binding = 1) uniform ViewProjectionMatrix {
  mat4 matrix;
} u_viewprojection;

void main() {
  gl_Position = u_viewprojection.matrix * u_model.matrix * vec4(a_pos, 1);
  v_colour = a_pos + 0.5;
  v_texcoord = a_texcoord;
}

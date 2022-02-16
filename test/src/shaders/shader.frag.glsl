#version 450

layout(location = 0) in vec3 v_colour;

layout(location = 0) out vec4 o_colour;

layout(binding = 2) uniform sampler2D u_diffuse_texture;
layout(binding = 3) uniform sampler2D u_normal_texture;

void main() {
  o_colour = vec4(v_colour, 1.0);
}

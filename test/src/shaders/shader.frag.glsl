#version 450

layout(location = 0) in vec3 v_colour;
layout(location = 1) in vec2 v_texcoord;

layout(location = 0) out vec4 o_colour;

layout(binding = 2) uniform sampler2D u_diffuse_texture;

void main() {
  o_colour = texture(u_diffuse_texture, v_texcoord);
}

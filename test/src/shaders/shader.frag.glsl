#version 450

layout(location = 0) in vec3 v_colour;
layout(location = 1) in vec2 v_texcoord;

layout(location = 0) out vec4 o_colour;

layout(binding = 0) uniform sampler2D u_diffuse_texture;
layout(binding = 1) uniform sampler2D u_second_texture;

void main() {
  o_colour = texture(u_diffuse_texture, v_texcoord) * texture(u_second_texture, v_texcoord); // * vec4(v_colour, 1.0);
}

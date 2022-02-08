#version 450

layout(location = 0) in vec3 a_pos;

layout(location = 0) out vec3 v_colour;

void main() {
  gl_Position = vec4(a_pos, 1);
  v_colour = a_pos + 0.5;
}

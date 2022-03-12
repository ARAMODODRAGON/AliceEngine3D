
static constexpr const char* c_basic_mesh_shader = R""(
#type vertex
#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 v_uv;

void main() {
	v_uv = a_uv;
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

#type fragment
#version 450 core
out vec4 fragColor;

in vec2 v_uv;

void main() {
	fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

)"";

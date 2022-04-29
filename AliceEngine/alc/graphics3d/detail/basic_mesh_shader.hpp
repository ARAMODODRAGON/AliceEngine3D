
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
out float v_dot;

const vec3 pointLight = vec3(10.0f, 9.0f, 5.0f);

void main() {
	vec3 norm = (u_model * vec4(a_normal, 1.0f)).xyz;
	vec3 dir = normalize(a_position - pointLight);
	v_dot = dot(dir, norm);
	v_uv = a_uv;
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

#type fragment
#version 450 core
out vec4 fragColor;

in vec2 v_uv;
in float v_dot;

void main() {
	fragColor = mix(vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), (v_dot + 1.0f) * 0.5f);
	//float dot_ = dot(v_normal, normalize(vec3(1.0f, 0.0f, 1.0f)));
	//fragColor = mix(vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), (dot_ + 1.0f) * 0.5f);
	//fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

)"";

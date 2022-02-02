#version 330 core

layout(location = 0) out vec4 output_color;

uniform sampler2D u_texture;

in vec2 v_uv;

void main()
{
	output_color = texture(u_texture, v_uv);
}
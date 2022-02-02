#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in float texture_id;
layout(location = 2) in vec2 texture_uv;
layout(location = 3) in vec4 texture_color;

uniform mat4 u_projection;

out float v_texture_id;
out vec2 v_texture_uv;
out vec4 v_texture_color;

void main()
{
    v_texture_id = texture_id;
    v_texture_uv = texture_uv;
    v_texture_color = texture_color;

    gl_Position = u_projection * vec4(position.xy, 0, 1);
}
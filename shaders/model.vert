#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_normal;

out vec2 v_texture_uv;
out vec3 v_normal;
out vec3 v_position;

void main()
{
    v_texture_uv = texture_uv;
    v_normal = normalize(mat3(u_normal) * normal);

    vec4 position_transformed = u_model * vec4(position, 1.0);
    v_position = position_transformed.xyz;

    gl_Position = u_projection * u_view * position_transformed;
}

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_normal;

out vec2 v_texture_uv;
out vec3 v_position;
out mat3 v_TBN;

void main()
{
    // pass texture uv to the fragment shader

    v_texture_uv = texture_uv;

    // calculate the tbn matrix and pass it to the fragment shader

    vec3 T = normalize(vec3(u_model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(u_model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(u_model * vec4(normal,    0.0)));
    v_TBN = mat3(T, B, N);

    // calculate the transformed position of the vertex

    vec4 position_transformed = u_model * vec4(position, 1.0);
    v_position = position_transformed.xyz;

    // model view projection

    gl_Position = u_projection * u_view * position_transformed;
}

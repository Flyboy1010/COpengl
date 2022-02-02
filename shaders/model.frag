#version 330 core

layout(location = 0) out vec4 output_color;

uniform sampler2D u_texture_diffuse;

in vec2 v_texture_uv;
in vec3 v_normal;
in vec3 v_position;

void main()
{
    // light properties

    vec3 light_pos = vec3(20.0, 30.0, 30.0);
    vec3 light_direction = normalize(light_pos - v_position);

    // ambient light

    float ambient_intensity = 0.2;
    vec3 ambient_color = vec3(texture(u_texture_diffuse, v_texture_uv));
    vec3 ambient = ambient_intensity * ambient_color;

    // diffuse light

    float diffuse_intensity = clamp(dot(v_normal, light_direction), 0.0, 1.0);
    vec3 diffuse_color = vec3(texture(u_texture_diffuse, v_texture_uv));
    vec3 diffuse = diffuse_intensity * diffuse_color;

    // output color

    output_color = vec4(ambient + diffuse, 1.0);
}
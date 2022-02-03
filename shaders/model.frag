#version 330 core

layout(location = 0) out vec4 output_color;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_normal;
uniform sampler2D u_texture_specular;
uniform vec3 u_camera_position;

in vec2 v_texture_uv;
in vec3 v_position;
in mat3 v_TBN;

void main()
{
    // light properties

    vec3 light_pos = vec3(20.0, 30.0, 30.0);
    vec3 light_direction = normalize(light_pos - v_position);

    // calculate fragment normal with the the normal map and the TBN matrix

    vec3 fragment_normal = vec3(texture(u_texture_normal, v_texture_uv)) * 2 - 1.0;
    vec3 normal = normalize(v_TBN * fragment_normal); 

    // ambient light

    float ambient_intensity = 0.2;
    vec3 ambient_color = vec3(texture(u_texture_diffuse, v_texture_uv));
    vec3 ambient = ambient_intensity * ambient_color;

    // diffuse light

    float diffuse_intensity = clamp(dot(normal, light_direction), 0.0, 1.0);
    vec3 diffuse_color = vec3(texture(u_texture_diffuse, v_texture_uv));
    vec3 diffuse = diffuse_intensity * diffuse_color;

    // specular light

    vec3 view_direction = normalize(u_camera_position - v_position);
    float specular_strength = 0.5;
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_intensity = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * specular_intensity * vec3(texture(u_texture_specular, v_texture_uv));

    // output color

    output_color = vec4(ambient + diffuse + specular, 1.0);
}
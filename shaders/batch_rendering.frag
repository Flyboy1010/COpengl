#version 330 core

layout(location = 0) out vec4 output_color;

uniform sampler2D u_textures[32];

in float v_texture_id;
in vec2 v_texture_uv;
in vec4 v_texture_color;

void main()
{
    int texture_id = int(v_texture_id);

    switch (texture_id)
    {
        case 0: output_color = texture(u_textures[0], v_texture_uv) * v_texture_color; break;
        case 1: output_color = texture(u_textures[1], v_texture_uv) * v_texture_color; break;
        case 2: output_color = texture(u_textures[2], v_texture_uv) * v_texture_color; break;
        case 3: output_color = texture(u_textures[3], v_texture_uv) * v_texture_color; break;
        case 4: output_color = texture(u_textures[4], v_texture_uv) * v_texture_color; break;
        case 5: output_color = texture(u_textures[5], v_texture_uv) * v_texture_color; break;
        case 6: output_color = texture(u_textures[6], v_texture_uv) * v_texture_color; break;
        case 7: output_color = texture(u_textures[7], v_texture_uv) * v_texture_color; break;
        case 8: output_color = texture(u_textures[8], v_texture_uv) * v_texture_color; break;
        case 9: output_color = texture(u_textures[9], v_texture_uv) * v_texture_color; break;
        case 10: output_color = texture(u_textures[10], v_texture_uv) * v_texture_color; break;
        case 11: output_color = texture(u_textures[11], v_texture_uv) * v_texture_color; break;
        case 12: output_color = texture(u_textures[12], v_texture_uv) * v_texture_color; break;
        case 13: output_color = texture(u_textures[13], v_texture_uv) * v_texture_color; break;
        case 14: output_color = texture(u_textures[14], v_texture_uv) * v_texture_color; break;
        case 15: output_color = texture(u_textures[15], v_texture_uv) * v_texture_color; break;
        case 16: output_color = texture(u_textures[16], v_texture_uv) * v_texture_color; break;
        case 17: output_color = texture(u_textures[17], v_texture_uv) * v_texture_color; break;
        case 18: output_color = texture(u_textures[18], v_texture_uv) * v_texture_color; break;
        case 19: output_color = texture(u_textures[19], v_texture_uv) * v_texture_color; break;
        case 20: output_color = texture(u_textures[20], v_texture_uv) * v_texture_color; break;
        case 21: output_color = texture(u_textures[21], v_texture_uv) * v_texture_color; break;
        case 22: output_color = texture(u_textures[22], v_texture_uv) * v_texture_color; break;
        case 23: output_color = texture(u_textures[23], v_texture_uv) * v_texture_color; break;
        case 24: output_color = texture(u_textures[24], v_texture_uv) * v_texture_color; break;
        case 25: output_color = texture(u_textures[25], v_texture_uv) * v_texture_color; break;
        case 26: output_color = texture(u_textures[26], v_texture_uv) * v_texture_color; break;
        case 27: output_color = texture(u_textures[27], v_texture_uv) * v_texture_color; break;
        case 28: output_color = texture(u_textures[28], v_texture_uv) * v_texture_color; break;
        case 29: output_color = texture(u_textures[29], v_texture_uv) * v_texture_color; break;
        case 30: output_color = texture(u_textures[30], v_texture_uv) * v_texture_color; break;
        case 31: output_color = texture(u_textures[31], v_texture_uv) * v_texture_color; break;
    }
}
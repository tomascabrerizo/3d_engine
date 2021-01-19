#version 330 core

out vec4 FragColor;
in vec2 tex_coords;

uniform bool has_texture;
uniform vec3 ren_color;
uniform vec3 light_color;
uniform sampler2D ren_texture;

void main()
{
    if(has_texture)
        FragColor = texture(ren_texture, tex_coords) * vec4(light_color, 1.0);
    else
        FragColor = vec4(ren_color, 1.0);
} 

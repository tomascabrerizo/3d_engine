#version 330 core

out vec4 FragColor;
in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normals;

uniform bool has_texture;
uniform vec3 ren_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

uniform sampler2D ren_texture;

void main()
{
    //Ambient light
    float ambient_strenght = 0.1;
    vec3 ambient_light = ambient_strenght * light_color;
    //Diffuse light
    vec3 norm = normalize(normals);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;
    //Specular light
    float specular_strenght = 0.7;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflec_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflec_dir), 0.0), 32);
    vec3 specular = specular_strenght * spec * light_color;

    if(has_texture)
        FragColor = texture(ren_texture, tex_coords) * 
            vec4(light_color * (ambient_light + diffuse + specular), 1.0);
    else
        FragColor = vec4(ren_color, 1.0);
} 

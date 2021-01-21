#version 330 core

out vec4 FragColor;
in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normals;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform bool has_texture;
uniform vec3 ren_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform Material material;
uniform Light light;

uniform sampler2D ren_texture;

void main()
{
    //Ambient light
    vec3 ambient = light.ambient * material.ambient;
    //Diffuse light
    vec3 norm = normalize(normals);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    //Specular light
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflec_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflec_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    //TODO(tomi): make this if in two differents shaders
    if(has_texture)
        FragColor = texture(ren_texture, tex_coords) * 
            vec4(light_color * (ambient + diffuse + specular), 1.0);
    else
        FragColor = vec4(ren_color, 1.0);
} 

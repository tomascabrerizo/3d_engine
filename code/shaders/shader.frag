#version 330 core

out vec4 FragColor;
in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normals;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform bool has_texture;
uniform vec3 ren_color;
uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main()
{
    //Ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
    //Diffuse light
    vec3 norm = normalize(normals);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    //Specular light
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflec_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflec_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));
    
    //TODO(tomi): make this if in two differents shaders
    if(has_texture)
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    else
        FragColor = vec4(ren_color, 1.0);
} 

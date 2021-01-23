#version 330 core

out vec4 FragColor;
in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normals;

uniform vec3 view_pos;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct DirLight {

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dir_light;
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_POINT_LIGHTS 4
uniform int NR_POINT_LIGHTS;
uniform PointLight point_lights[MAX_NR_POINT_LIGHTS];
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);


void main()
{
    vec3 norm = normalize(normals);
    vec3 view_dir = normalize(view_pos - frag_pos);
    //Directional light
    vec3 result = calc_dir_light(dir_light, norm, view_dir);
    //Points lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
    }

    FragColor = vec4(result, 1.0);
} 

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflec_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflec_dir), 0.0f), material.shininess);
    //Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflec_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflec_dir), 0.0f), material.shininess);
    //Attenuation
    float dis = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));
    //Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

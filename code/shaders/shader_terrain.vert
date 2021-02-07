#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 tex_coords;
out vec3 normals;
out vec3 frag_pos;
out float visibility;

const float density = 0.004;
const float gradient = 5.0;

void main()
{
    vec4 position_rel_cam = view * model * vec4(aPos, 1.0);
    gl_Position = projection * position_rel_cam; 
    tex_coords = aTex;
    normals = mat3(transpose(inverse(model))) * aNor; 
    frag_pos = vec3(model * vec4(aPos, 1.0));
    
    float d_to_cam = length(position_rel_cam.xyz);
    visibility = exp(-pow(d_to_cam*density, gradient));
    //visibility = 1 - ((d_to_cam-f_near) / f_far); //My linear fog ecuation
    visibility = clamp(visibility, 0.0, 1.0);

}

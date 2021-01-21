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

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    tex_coords = aTex;
    normals = mat3(transpose(inverse(model))) * aNor; 
    frag_pos = vec3(model * vec4(aPos, 1.0));
}

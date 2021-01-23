#version 330 core
out vec4 FragColor;

uniform vec3 ren_color;

void main()
{
    FragColor = vec4(ren_color, 1.0f);
}

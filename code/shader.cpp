#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shader.h"
#include "utils.h"

uint32_t shader_create_program(const char* vert_path, const char* frag_path)
{
    int success;
    char infoLog[512];

    file_info vertex_shader_source = read_entire_file(vert_path);
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source.content, 0);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("ERROR(vertex_shader):%s\n", infoLog);
    };

    file_info fragment_shader_source = read_entire_file(frag_path);
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source.content, 0);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR(fragment_shader):%s\n", infoLog);
    };

    free(vertex_shader_source.content);
    free(fragment_shader_source.content);
    
    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR(program_shader):%s\n", infoLog);
    };

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader); 

    return shader_program;
}

void shader_use_program(uint32_t program)
{
    glUseProgram(program);
}

void shader_set_bool(uint32_t program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(program, name), (int)value);
}

void shader_set_int(uint32_t program, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(program, name), value);
}


void shader_set_float(uint32_t program, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(program, name), value);
}

void shader_set_v3(uint32_t program, const char* name, v3 v)
{
    glUniform3f(glGetUniformLocation(program, name), v.x, v.y, v.z);
}

void shader_set_m4(uint32_t program, const char* name, m4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, true, m.m[0]);
}

void shader_set_material(uint32_t program, const char* name, const Material& m)
{
    char u_name[256];
    strcpy(u_name, name); 
    strcat(u_name, ".ambient");
    shader_set_v3(program, u_name, m.ambient);
    strcpy(u_name, name); 
    strcat(u_name, ".diffuse");
    shader_set_v3(program, u_name, m.diffuse);
    strcpy(u_name, name); 
    strcat(u_name, ".specular");
    shader_set_v3(program, u_name, m.specular);
    strcpy(u_name, name); 
    strcat(u_name, ".shininess");
    shader_set_float(program, u_name, m.shininess);
}

void shader_set_light(uint32_t program, const char* name, const Light& l)
{
    char u_name[256];
    strcpy(u_name, name); 
    strcat(u_name, ".position");
    shader_set_v3(program, u_name, l.position);
    strcpy(u_name, name); 
    strcat(u_name, ".ambient");
    shader_set_v3(program, u_name, l.ambient);
    strcpy(u_name, name); 
    strcat(u_name, ".diffuse");
    shader_set_v3(program, u_name, l.diffuse);
    strcpy(u_name, name); 
    strcat(u_name, ".specular");
    shader_set_v3(program, u_name, l.specular);

}

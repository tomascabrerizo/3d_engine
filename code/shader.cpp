#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include "shader.h"
#include "utils.h"

uint32_t shader_create_program(const char* vert_path, const char* frag_path)
{
    file_info vertex_shader_source = read_entire_file(vert_path);
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source.content, 0);
    glCompileShader(vertex_shader);

    file_info fragment_shader_source = read_entire_file(frag_path);
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source.content, 0);
    glCompileShader(fragment_shader);

    free(vertex_shader_source.content);
    free(fragment_shader_source.content);
    
    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

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

#ifndef SHADER_H
#define SHADER_H

#include <stdint.h>
#include "math.h"

uint32_t shader_create_program(const char* vert_path, const char* frag_path);
void shader_use_program(uint32_t program);
void shader_set_bool(uint32_t program, const char* name, bool value);
void shader_set_int(uint32_t program, const char* name, int value);
void shader_set_float(uint32_t program, const char* name, float value);
void shader_set_m4(uint32_t program, const char* name, m4 m);
void shader_set_v3(uint32_t program, const char* name, v3 v);

#endif

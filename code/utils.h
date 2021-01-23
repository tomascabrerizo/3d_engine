#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define array_count(x) (sizeof(x)/sizeof(x[0]))

typedef struct
{
    char* content;
    uint32_t size;
    
} file_info;

file_info read_entire_file(const char* path);
char* concat_string(const char* s0, const char* s1, char* s);

#endif

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

typedef struct
{
    char* content;
    uint32_t size;
    
} file_info;

file_info read_entire_file(const char* path);

#endif

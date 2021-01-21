#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

file_info read_entire_file(const char* path)
{
    
    file_info result = {};
    FILE* file = fopen(path, "rb");
    if(!file)
    {
	printf("cannot find file (%s)", path);
	return result;
    }
    
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* content = (char*)malloc(size+1);
    fread(content, 1, size, file);
    fclose(file);
    content[size] = 0;

    result.content = content;
    result.size = size;
    return result;
}

char* concat_string(const char* s0, const char* s1, char* s)
{
    strcpy(s, s0); 
    strcat(s, s1);
    return s;
}

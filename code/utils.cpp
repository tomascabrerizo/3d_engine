#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "ctype.h"

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

String_View cstr_as_sv(const char* cstr)
{
    String_View sv = {};
    sv.count = strlen(cstr);
    sv.data = cstr;
    return sv;
} 

String_View sv_trim_left(const String_View& sv)
{
    uint32_t i = 0;
    while(i < sv.count && isspace(sv.data[i]))
    {
        i += 1;
    }
    String_View r = {};
    r.count = sv.count - i;
    r.data = sv.data + i;
    return r;
}

String_View sv_trim_right(const String_View& sv)
{
    uint32_t i = 0;
    while(i < sv.count && isspace(sv.data[sv.count-1-i]))
    {
        i += 1;
    }
    String_View r = {};
    r.count = sv.count - i;
    r.data = sv.data;
    return r;
}

String_View sv_trim(const String_View& sv)
{
    return sv_trim_right(sv_trim_left(sv)); 
}

String_View sv_chop_by_delim(String_View* sv, char delim)
{
    uint32_t i = 0;
    while(i < sv->count && sv->data[i] != delim)
    {
        i++;
    }
    String_View r = {};
    r.count = i;
    r.data = sv->data;
    if(i < sv->count)
    {
        sv->count -= i + 1;
        sv->data  += i + 1;
    }
    return r;
}

void sv_print(const String_View& sv)
{
    fprintf(stdout, "%.*s\n", sv.count, sv.data);
}

bool sv_equals(const String_View& sv,const char* cstr)
{
    if(sv.count != strlen(cstr))
    {
        return false;
    }
    return strncmp(sv.data, cstr, sv.count) == 0;
}


bool sv_empty(const String_View& sv)
{
    return sv.count == 0;
}

//TODO(tomi): this function should return a model
void parse_obj_file(const char* path)
{
    //TODO(tomi): create my own container to save the important data
    //std::vector<Vertex> lines;
    //std::vector<TextCoords> lines;
    //std::vector<Normals> lines;
    FILE* data = fopen(path, "r");
    char line[256];
    while(fgets(line, sizeof(line), data))
    {
        String_View sv_line = sv_trim(cstr_as_sv(line));
        if(sv_empty(sv_line)) continue;
        
        String_View header = sv_trim(sv_chop_by_delim(&sv_line, ' '));
        sv_line = sv_trim(sv_line);
        if(sv_equals(header, "v"))
        {
            printf("vertex\n");
        }
        else if(sv_equals(header, "vt"))
        {
            printf("text coords\n");
        }
        else
        {
            continue;
        }
        
    } 
    printf("ready!\n");
}

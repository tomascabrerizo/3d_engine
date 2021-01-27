#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include "math.h"
#include "utils.h"
#include "game_state.h"

file_info read_entire_file(const char* path)
{
    
    file_info result = {};
    FILE* file = fopen(path, "rb");
    if(!file)
    {
        printf("cannot find file (%s)\n", path);
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

float sv_to_float(String_View sv)
{
    char cstr_number[256];
    sprintf(cstr_number, "%.*s", sv.count, sv.data);
    float number = atof(cstr_number);
    return number;
}


int sv_to_int(String_View sv)
{
    char cstr_number[256];
    sprintf(cstr_number, "%.*s", sv.count, sv.data);
    int number = atoi(cstr_number);
    return number;
}

void load_obj_file(const char* path, GameState* gs)
{
    
    FILE* data = fopen(path, "r");
    if(!data)
    {
        printf("cannot find file (%s)\n", path);
        return;
    }
    
    char line[512];
    std::vector<v3> vertices;
    std::vector<v2> text_coords;
    std::vector<v3> normals;

    while(fgets(line, sizeof(line), data))
    {
        //TODO(tomi):Try to implements obj parser without StringView and std::vector
        String_View sv_line = sv_trim(cstr_as_sv(line));
        if(sv_empty(sv_line)) continue;
        
        String_View header = sv_trim(sv_chop_by_delim(&sv_line, ' '));
        sv_line = sv_trim(sv_line);
        if(sv_equals(header, "o"))
        {
            gs->last_mesh_index++;
        }
        else if(sv_equals(header, "v"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s2 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            float f2 = sv_to_float(s2);
            vertices.push_back(new_v3(f0, f1, f2));
        }
        else if(sv_equals(header, "vt"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            text_coords.push_back(new_v2(f0, f1));
        }
        else if(sv_equals(header, "vn"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s2 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            float f2 = sv_to_float(s2);
            normals.push_back(new_v3(f0, f1, f2));
        }
        else if(sv_equals(header, "f"))
        {
            String_View s_vertex0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s_vertex1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s_vertex2 = sv_trim(sv_line);
            
            String_View sv0  = sv_chop_by_delim(&s_vertex0, '/');
            String_View svt0 = sv_chop_by_delim(&s_vertex0, '/');
            String_View svn0 = sv_trim(s_vertex0);

            String_View sv1  = sv_chop_by_delim(&s_vertex1, '/');
            String_View svt1 = sv_chop_by_delim(&s_vertex1, '/');
            String_View svn1 = sv_trim(s_vertex1);
            
            String_View sv2  = sv_chop_by_delim(&s_vertex2, '/');
            String_View svt2 = sv_chop_by_delim(&s_vertex2, '/');
            String_View svn2 = sv_trim(s_vertex2);

            int v0_index = sv_to_int(sv0)-1;
            int v1_index = sv_to_int(sv1)-1;
            int v2_index = sv_to_int(sv2)-1;
            
            int vt0_index = sv_to_int(svt0)-1;
            int vt1_index = sv_to_int(svt1)-1;
            int vt2_index = sv_to_int(svt2)-1;

            int vn0_index = sv_to_int(svn0)-1;
            int vn1_index = sv_to_int(svn1)-1;
            int vn2_index = sv_to_int(svn2)-1;
            
            Vertex new_vertex0 = new_vertex(vertices[v0_index], normals[vn0_index], text_coords[vt0_index]);
            Vertex new_vertex1 = new_vertex(vertices[v1_index], normals[vn1_index], text_coords[vt1_index]);
            Vertex new_vertex2 = new_vertex(vertices[v2_index], normals[vn2_index], text_coords[vt2_index]);
            
            gs->game_meshes[gs->last_mesh_index].vertices.push_back(new_vertex0);
            gs->game_meshes[gs->last_mesh_index].vertices.push_back(new_vertex1);
            gs->game_meshes[gs->last_mesh_index].vertices.push_back(new_vertex2);
        }
    }
    printf("ready!\n");
}

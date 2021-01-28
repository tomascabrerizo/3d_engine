#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>

#define array_count(x) (sizeof(x)/sizeof(x[0]))

struct GameState;

struct file_info
{
    char* content;
    uint32_t size;
    
};

struct String_View {
    uint32_t count;
    const char* data;
};

String_View cstr_as_sv(const char* cstr);
String_View sv_trim_left(const String_View& sv);
String_View sv_trim_right(const String_View& sv);
String_View sv_trim(const String_View& sv);
String_View sv_chop_by_delim(String_View* sv, char delim);

//TODO(tomi):Make this two functions have const & param
float sv_to_float(String_View sv);
int sv_to_int(String_View sv);

bool sv_empty(const String_View& sv);
bool sv_equals(const String_View& sv, const char* cstr);
void sv_print(const String_View& sv);

file_info read_entire_file(const char* path);
char* concat_string(const char* s0, const char* s1, char* s);

#endif

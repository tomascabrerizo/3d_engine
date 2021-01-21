#ifndef LIGHT_H
#define LIGHT_H

#include "math.h"

struct Light 
{
    v3 position;
    v3 ambient;
    v3 diffuse;
    v3 specular;
};

Light light_create(v3 pos, v3 ambient, v3 diffuse, v3 specular);

#endif

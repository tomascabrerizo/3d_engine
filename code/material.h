#ifndef MATERIAL_H
#define MATERIAL_H

#include "math.h"

struct Material
{
    v3 ambient;
    //v3 diffuse;
    v3 specular;
    float shininess;
};

Material material_create(v3 ambient, v3 specular, float shininess);

#endif

#ifndef MATERIAL_H
#define MATERIAL_H

#include "math.h"
#include "defines.h"

struct Material
{
    v3 ambient;
    v3 diffuse;
    v3 specular;
    float shininess;
    TextureIndex texture_index;
    TextureIndex texture_specular_index;
};

Material material_create(TextureIndex t_index, TextureIndex ts_index, float shininess);
Material material_create(v3 ambient, v3 diffuse, v3 specular, float shininess);

#endif
